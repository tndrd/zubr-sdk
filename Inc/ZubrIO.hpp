#pragma once

#include "BaseSerial.hpp"
#include "CsMessage.h"
#include <array>
#include <utility>

namespace Zubr {
class IO {
public:
  using TTYConfig = BaseSerial::TTYConfig;

private:
  static constexpr size_t MaxPacketSize = 64 - 1;
  static constexpr size_t RxBufSize = 512;

public:
  #pragma warning "CHANGE ME" 

  BaseSerial Serial;
  std::array<uint8_t, RxBufSize> RxBuf;

public:
  IO(const TTYConfig &ttyConf) : Serial{ttyConf} {}

  void Send(const CsMessageOut &msg) const {
    const uint8_t *buf = reinterpret_cast<const uint8_t *>(msg.buffer());
    size_t len = msg.length();
    size_t i = 0;

    for (; i < len / MaxPacketSize; i += MaxPacketSize) {
      Serial.Send(buf + i, MaxPacketSize);
    }

    Serial.Send(buf + i, len % MaxPacketSize);
  }

  CsMessageIn Recv(int16_t rspSz) {
    if (rspSz > RxBufSize)
      throw std::runtime_error("Recv(): message size is too big");
    /*
    size_t i = 0;
    for (; i < rspSz / MaxPacketSize; i += MaxPacketSize) {
      Serial.Recv(RxBuf.data() + i, MaxPacketSize);
    }
    */

    Serial.Recv(RxBuf.data(), rspSz + 1);

    CsMessageIn msg{reinterpret_cast<const char *>(RxBuf.data()), 0, rspSz, 1};

    if (!msg.checkCrc(rspSz))
      throw std::runtime_error("Recv(): Bad CS");

    return msg;
  }
};
} // namespace Zubr