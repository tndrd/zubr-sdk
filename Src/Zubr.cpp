#include "zubr-sdk/Zubr.hpp"

namespace Zubr {

IOHandle Zubr::MakeIO(const std::string& port) {
  IOHandle::TTYConfig conf;

  conf.Baudrate = 3000000;
  conf.ParityBit = false;
  conf.Stopbits = IOHandle::TTYConfig::StopbitsCount::One;
  conf.Timeout = 1;

  conf.Port = port;

  return {conf};
}

Zubr::Zubr(const std::string& port) : IO{MakeIO(port)} {}

auto Zubr::SetPosition(const State& target) -> State {
  auto encoded = Converter::Encode(target);
  auto result = Call<Procedures::SetPosition>(encoded);
  return Converter::Decode(result);
}

auto Zubr::GetPosition() -> State {
  auto result = RPC::Call<Procedures::GetPosition>(IO);
  return Converter::Decode(result);
}

auto Zubr::GetVelocity() -> State {
  auto result = Call<Procedures::GetVelocity>();
  return Converter::Decode(result);
}

auto Zubr::GetIMU() -> IMU {
  auto result = Call<Procedures::GetIMU>();
  return Converter::Decode(result);
}

auto Zubr::GetControllerInfo() -> ControllerInfo {
  auto result = Call<Procedures::GetControllerInfo>();
  return Converter::Decode(result);
}

void Zubr::StartSlot(int8_t slot) { Call<Procedures::StartSlot>({slot}); }

double Zubr::GetBatteryLevel() {
  // The only hardcoded index we are using now
  // No need to move it to header IMO, as user
  // wouldn't ever need it
  static constexpr int16_t BatteryLevelParamIndex = 2;

  auto param = Call<Procedures::GetZubrParam>(BatteryLevelParamIndex);
  return Converter::DecodeValue::BatteryLevel(param.Value());
}

}  // namespace Zubr