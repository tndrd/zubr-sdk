#include "zubr-sdk/Converter.hpp"

namespace Zubr {
double Converter::DecodeValue::ServoAngle(int16_t value) {
  return value / Constants::ServoNorm;
}

double Converter::DecodeValue::ServoSpeed(int16_t value) {
  return value / Constants::SpeedNorm;
}

double Converter::DecodeValue::QuatComponent(int16_t value) {
  return value / Constants::QuatNorm;
}
double Converter::DecodeValue::GyroComponent(int16_t value) {
  return value / Constants::GyroNorm;
}
double Converter::DecodeValue::AcclComponent(int16_t value) {
  return value / Constants::AcclNorm;
}

std::string Converter::DecodeValue::ControllerName(
    RPC::Messages::ContrNameBuf buf) {
  buf.back() = '\0';
  return reinterpret_cast<char*>(buf.data());
}

int16_t Converter::EncodeValue::ServoAngle(double rad) {
  return rad * Constants::ServoNorm;
}

int16_t Converter::EncodeValue::QuatComponent(double value) {
  return value * Constants::QuatNorm;
}

int16_t Converter::EncodeValue::GyroComponent(double value) {
  return value * Constants::GyroNorm;
}

int16_t Converter::EncodeValue::AcclComponent(double value) {
  return value * Constants::AcclNorm;
}

auto Converter::Decode(const EncodedMsgs::IMU& imu) -> DecodedMsgs::IMU {
  DecodedMsgs::IMU decoded;

  decoded.Quat.X = DecodeValue::QuatComponent(imu.Quat().X);
  decoded.Quat.Y = DecodeValue::QuatComponent(imu.Quat().Y);
  decoded.Quat.Z = DecodeValue::QuatComponent(imu.Quat().Z);
  decoded.Quat.W = DecodeValue::QuatComponent(imu.Quat().W);

  decoded.Gyro.X = DecodeValue::GyroComponent(imu.Gyro().X);
  decoded.Gyro.Y = DecodeValue::GyroComponent(imu.Gyro().Y);
  decoded.Gyro.Z = DecodeValue::GyroComponent(imu.Gyro().Z);

  decoded.Accl.X = DecodeValue::AcclComponent(imu.Accl().X);
  decoded.Accl.Y = DecodeValue::AcclComponent(imu.Accl().Y);
  decoded.Accl.Z = DecodeValue::AcclComponent(imu.Accl().Z);

  return decoded;
}

template <typename Decoder>
auto Converter::DecodeState(const EncodedMsgs::State& state, Decoder decoder)
    -> DecodedMsgs::State {
  DecodedMsgs::State decoded;
  decoded.Frame = state.Frame();

  for (int i = 0; i < state.Values().size(); ++i)
    decoded.Values[i] = decoder(state.Values()[i]);

  return decoded;
}

struct AngleDecoder {
  double operator()(int16_t value) {
    return Converter::DecodeValue::ServoAngle(value);
  }
};

struct SpeedDecoder {
  double operator()(int16_t value) {
    return Converter::DecodeValue::ServoSpeed(value);
  }
};

auto Converter::Decode(const EncodedMsgs::Angles& angles) -> DecodedMsgs::State {
  return DecodeState(angles, AngleDecoder{});
}

auto Converter::Decode(const EncodedMsgs::Speeds& speeds) -> DecodedMsgs::State {
  return DecodeState(speeds, SpeedDecoder{});
}

auto Converter::Decode(const EncodedMsgs::ControllerInfo& info)
    -> DecodedMsgs::ControllerInfo {
  DecodedMsgs::ControllerInfo decoded;

  decoded.ControllerName = DecodeValue::ControllerName(info.ControllerName());

  decoded.FlashSize = info.FlashSize();
  decoded.FwareVersion = info.FwareVersion();
  decoded.RobotID = info.RobotID();
  decoded.RobotSerial = info.RobotSerial() + Constants::RobotSerialBase;
  decoded.RobotVersion = info.RobotVersion() + Constants::RobotVersionBase;

  return decoded;
}

auto Converter::Encode(const DecodedMsgs::State& state) -> EncodedMsgs::Angles {
  EncodedMsgs::Angles encoded;
  encoded.Frame() = state.Frame;

  for (int i = 0; i < state.Values.size(); ++i)
    encoded.Values()[i] = EncodeValue::ServoAngle(state.Values[i]);

  return encoded;
}
}  // namespace Zubr