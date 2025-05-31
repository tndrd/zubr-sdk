#include "zubr-sdk/ZubrIO.hpp"

#include <cassert>
#include <stdexcept>

namespace Zubr {
IOHandle::IOHandle(const TTYConfig &ttyConf) : Serial{ttyConf} {}

void IOHandle::Send(const CsMessageOut &msg) const {
  const uint8_t *buf = reinterpret_cast<const uint8_t *>(msg.buffer());
  size_t len = msg.length();
  assert(len < MaxPacketSize &&
         "len >= MaxPacketSize logic is not implemented");

  Serial.Send(buf, len);
}

CsMessageIn IOHandle::Recv(int16_t rspSz) {
  if (rspSz > RxBufSize)
    throw std::runtime_error("Recv(): message size is too big");
  assert(rspSz < MaxPacketSize &&
         "rspSz >= MaxPacketSize logic is not implemented");

  Serial.Recv(RxBuf.data(), rspSz + 1);

  CsMessageIn msg{reinterpret_cast<const char *>(RxBuf.data()), 0, rspSz, 1};

  if (!msg.checkCrc(rspSz)) throw std::runtime_error("Recv(): Bad CS");

  return msg;
}
}  // namespace Zubr