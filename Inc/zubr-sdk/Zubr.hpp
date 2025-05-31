#pragma once
#include "zubr-sdk/Converter.hpp"
#include "zubr-sdk/ZubrRPC.hpp"
#include "zubr-sdk/Printers.hpp"

namespace Zubr {
class Zubr {
 public:
  using State = Converter::DecodedMsgs::State;
  using IMU = Converter::DecodedMsgs::IMU;
  using Procedures = RPC::Procedures;

 private:
  IOHandle IO;

 private:
  static IOHandle MakeIO(const std::string& port);

 public:
  Zubr(const std::string& port);

  State SetPosition(const State& target);
  State GetPosition();
  State GetVelocity();
  IMU GetIMU();
  void StartSlot(int8_t slot);

  template <typename Proc>
  typename Proc::Responce Call(const typename Proc::Request& request = {}) {
    return RPC::Call<Proc>(IO, request);
  }
};
}  // namespace Zubr