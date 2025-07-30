#pragma once
#include "zubr-sdk/Converter.hpp"
#include "zubr-sdk/Printers.hpp"
#include "zubr-sdk/ZubrRPC.hpp"

namespace Zubr {
class Zubr {
 public:
  using State = Converter::DecodedMsgs::State;
  using IMU = Converter::DecodedMsgs::IMU;
  using ControllerInfo = Converter::DecodedMsgs::ControllerInfo;

  using Procedures = RPC::Procedures;

 private:
  IOHandle IO;

 private:
  static IOHandle MakeIO(const std::string& port);

 public:
  // @brief Construct handle using device port
  // @param port Device port (usually /dev/ttyACM0)
  Zubr(const std::string& port);

  // @brief Set positions of all servos
  // @param target Target positions
  // @return Latest positions
  State SetPosition(const State& target);

  // @brief Get latest servo positions
  // @return Latest positions
  State GetPosition();

  // @brief Get latest servo velocities
  // @return Latest velocities
  State GetVelocity();

  // @brief Get latest IMU data
  // @return Latest velocities
  IMU GetIMU();

  // @brief Start Zubr-controller slot
  // @param Slot ID
  void StartSlot(int8_t slot);

  // @brief Get current battery level
  // @return Battery level in volts
  double GetBatteryLevel();

  // @brief Get controller info
  // @return Controller info
  ControllerInfo GetControllerInfo();

  // @brief Generic interface for extended Zubr remote procedure calls
  template <typename Proc>
  typename Proc::Responce Call(const typename Proc::Request& request = {}) {
    return RPC::Call<Proc>(IO, request);
  }
};
}  // namespace Zubr