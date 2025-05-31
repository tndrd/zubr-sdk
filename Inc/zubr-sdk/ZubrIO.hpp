#pragma once

#include <array>

#include "zubr-sdk/BaseSerial.hpp"
#include "zubr-sdk/CsMessage.h"

namespace Zubr {
class IOHandle {
 public:
  using TTYConfig = BaseSerial::TTYConfig;

 private:
  static constexpr size_t MaxPacketSize = 64 - 1;
  static constexpr size_t RxBufSize = 512;

 private:
  BaseSerial Serial;
  std::array<uint8_t, RxBufSize> RxBuf;

 public:
  IOHandle(const TTYConfig &ttyConf);
  void Send(const CsMessageOut &msg) const;
  CsMessageIn Recv(int16_t rspSz);
};
}  // namespace Zubr