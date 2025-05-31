#pragma once
#include <string>

namespace Zubr {
class BaseSerial final {
 private:
  struct Descriptor final {
   private:
    int Fd = -1;

   public:
    explicit Descriptor(int newFd = -1);

    int Get() const;
    bool IsValid() const;

    Descriptor(const Descriptor &) = delete;
    Descriptor &operator=(const Descriptor &) = delete;

    Descriptor(Descriptor &&rhs);
    Descriptor &operator=(Descriptor &&rhs);

    ~Descriptor();
  };

 public:
  struct TTYConfig final {
    std::string Port;
    size_t Baudrate;

    enum class StopbitsCount { One, Two };
    StopbitsCount Stopbits;

    bool ParityBit;
    float Timeout;
  };

 private:
  Descriptor Fd;
  TTYConfig TTY;

  void Configure(const TTYConfig &ttyConfig);

 public:
  BaseSerial(const TTYConfig &config);

  BaseSerial(const BaseSerial &) = delete;
  BaseSerial &operator=(const BaseSerial &) = delete;

  BaseSerial(BaseSerial &&) = default;
  BaseSerial &operator=(BaseSerial &&) = default;

  void Send(const uint8_t *buf, size_t size) const;
  void Recv(uint8_t *buf, size_t size) const;
};

}  // namespace Zubr