#include "BaseSerial.hpp"

#include <asm/termbits.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <array>
#include <assert.h>
#include <iostream>
#include <string>
#include <vector>

namespace Zubr {

BaseSerial::Descriptor::Descriptor(int newFd) : Fd{newFd} {}

bool BaseSerial::Descriptor::IsValid() const { return Fd >= 0; }
int BaseSerial::Descriptor::Get() const { return Fd; }

BaseSerial::Descriptor::Descriptor(Descriptor &&rhs) : Fd{rhs.Fd} { rhs.Fd = -1; }

auto BaseSerial::Descriptor::operator=(Descriptor &&rhs) -> Descriptor & {
  std::swap(Fd, rhs.Fd);
  return *this;
}

BaseSerial::Descriptor::~Descriptor() {
  if (IsValid())
    close(Fd);
}

std::runtime_error MakeTTYException(const BaseSerial::TTYConfig &conf,
                                    const std::string &msg) {
  return std::runtime_error(conf.Port + ": " + msg);
}

std::runtime_error MakeTTYExceptionErrno(const BaseSerial::TTYConfig &conf,
                                         const std::string &msg) {
  return MakeTTYException(conf, msg + ": errno #" + std::to_string(errno) +
                                    ": " + strerror(errno));
}

void BaseSerial::Configure(const TTYConfig &ttyConfig) {
  Descriptor newFd{open(ttyConfig.Port.c_str(), O_RDWR)};

  if (!newFd.IsValid())
    throw MakeTTYExceptionErrno(ttyConfig, "Failed to open port");

  termios2 tty;

  if (ioctl(newFd.Get(), TCGETS2, &tty))
    throw MakeTTYExceptionErrno(ttyConfig,
                                "ioctl(): Failed to get tty settings");

  tty.c_cflag &= ~CBAUD;
  tty.c_cflag |= CBAUDEX;
  tty.c_ispeed = ttyConfig.Baudrate;
  tty.c_ospeed = ttyConfig.Baudrate;

  if (ttyConfig.Stopbits == TTYConfig::StopbitsCount::One)
    tty.c_cflag &= ~CSTOPB;
  if (ttyConfig.Stopbits == TTYConfig::StopbitsCount::Two)
    tty.c_cflag |= CSTOPB;

  if (!ttyConfig.ParityBit)
    tty.c_cflag &= ~PARENB;
  if (ttyConfig.ParityBit)
    tty.c_cflag |= PARENB;

  if (ttyConfig.Timeout > 25) // Maximum posix timeout is 25.5 seconds
    throw MakeTTYException(ttyConfig, "Timeout should be < 25s");

  uint8_t vtime = static_cast<uint8_t>(ttyConfig.Timeout * 10);

  tty.c_cc[VTIME] = vtime;
  tty.c_cc[VMIN] = 0;

  tty.c_lflag &= ~ICANON;
  tty.c_iflag &= ~(IXON | IXOFF | IXANY);
  tty.c_lflag &= ~ECHO;   // Disable echo
  tty.c_lflag &= ~ECHOE;  // Disable erasure
  tty.c_lflag &= ~ECHONL; // Disable new-line echo
  tty.c_lflag &= ~ISIG;   // Disable interpretation of INTR, QUIT and SUSP
  tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR |
                   ICRNL); // Disable any special handling of received bytes
  
  tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g.
                         // newline chars)
  tty.c_oflag &=
      ~ONLCR; // Prevent conversion of newline to carriage return/line feed

  if (ioctl(newFd.Get(), TCSETS2, &tty))
    throw MakeTTYExceptionErrno(ttyConfig, "ioctl(): Failed to set up");

  Fd = std::move(newFd);
  TTY = ttyConfig;
}

BaseSerial::BaseSerial(const TTYConfig &tty) { Configure(tty); }

void BaseSerial::Send(const uint8_t *buf, size_t size) const {
  if (!buf)
    throw MakeTTYException(TTY, "Send(): buf is NULL");

  size_t total = 0;
  while (total < size) {
    int ret = write(Fd.Get(), buf + total, size - total);
    if (ret < 0)
      throw MakeTTYExceptionErrno(TTY, "write()");
    else if (ret == 0)
      throw MakeTTYException(TTY, "Send(): Write timeout");
    total += ret;
  }
}

void BaseSerial::Recv(uint8_t *buf, size_t size) const {
  if (!buf)
    throw MakeTTYException(TTY, "Recv(): buf is NULL");

  size_t total = 0;
  while (total < size) {
    int ret = read(Fd.Get(), buf + total, size - total);
    if (ret < 0)
      throw MakeTTYExceptionErrno(TTY, "read()");
    else if (ret == 0)
      throw MakeTTYException(TTY, "Recv(): Read timeout");
    total += ret;
  }
}

} // namespace Zubr