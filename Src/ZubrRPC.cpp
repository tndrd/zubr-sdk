#include "zubr-sdk/ZubrRPC.hpp"

namespace Zubr {

template <>
void RPC::Messages::BaseTypes::Put(CsMessageOut &msg, const int8_t &val) {
  msg.addInt8(val);
}
template <>
void RPC::Messages::BaseTypes::Get(CsMessageIn &msg, int8_t &arg) {
  arg = msg.getInt8();
}

template <>
void RPC::Messages::BaseTypes::Put(CsMessageOut &msg, const int16_t &val) {
  msg.addInt16(val);
}
template <>
void RPC::Messages::BaseTypes::Get(CsMessageIn &msg, int16_t &arg) {
  arg = msg.getInt16();
}

template <>
void RPC::Messages::BaseTypes::Put(CsMessageOut &msg, const int32_t &val) {
  msg.addInt32(val);
}
template <>
void RPC::Messages::BaseTypes::Get(CsMessageIn &msg, int32_t &arg) {
  arg = msg.getInt32();
}

template <>
void RPC::Messages::BaseTypes::Put(CsMessageOut &msg, const StateVec &values) {
  for (const auto &val : values) Put(msg, val);
}
template <>
void RPC::Messages::BaseTypes::Get(CsMessageIn &msg, StateVec &arg) {
  for (int i = 0; i < arg.size(); ++i) Get(msg, arg[i]);
}

template <>
void RPC::Messages::BaseTypes::Put(CsMessageOut &msg, const XYZ &val) {
  Put(msg, val.X);
  Put(msg, val.Y);
  Put(msg, val.Z);
}

template <>
void RPC::Messages::BaseTypes::Get(CsMessageIn &msg, XYZ &arg) {
  Get(msg, arg.X);
  Get(msg, arg.Y);
  Get(msg, arg.Z);
}

template <>
void RPC::Messages::BaseTypes::Put(CsMessageOut &msg, const XYZW &val) {
  Put(msg, val.X);
  Put(msg, val.Y);
  Put(msg, val.Z);
  Put(msg, val.W);
}

template <>
void RPC::Messages::BaseTypes::Get(CsMessageIn &msg, XYZW &arg) {
  Get(msg, arg.X);
  Get(msg, arg.Y);
  Get(msg, arg.Z);
  Get(msg, arg.W);
}

};  // namespace Zubr