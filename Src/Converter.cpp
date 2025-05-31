#include "zubr-sdk/Converter.hpp"

namespace Zubr {
double Converter::DecodeValue::ServoAngle(int16_t value) {
  return value / ServoNorm;
}
double Converter::DecodeValue::QuatComponent(int16_t value) {
  return value / QuatNorm;
}
double Converter::DecodeValue::GyroComponent(int16_t value) {
  return value / GyroNorm;
}
double Converter::DecodeValue::AcclComponent(int16_t value) {
  return value / AcclNorm;
}

int16_t Converter::EncodeValue::ServoAngle(double rad) {
  return rad * ServoNorm;
}

int16_t Converter::EncodeValue::QuatComponent(double value) {
  return value * QuatNorm;
}

int16_t Converter::EncodeValue::GyroComponent(double value) {
  return value * GyroNorm;
}

int16_t Converter::EncodeValue::AcclComponent(double value) {
  return value * AcclNorm;
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

auto Converter::Decode(const EncodedMsgs::State& state) -> DecodedMsgs::State {
  DecodedMsgs::State decoded;
  decoded.Frame = state.Frame();

  for (int i = 0; i < state.Values().size(); ++i)
    decoded.Values[i] = DecodeValue::ServoAngle(state.Values()[i]);

  return decoded;
}

auto Converter::Encode(const DecodedMsgs::State& state) -> EncodedMsgs::State {
  EncodedMsgs::State encoded;
  encoded.Frame() = state.Frame;

  for (int i = 0; i < state.Values.size(); ++i)
    encoded.Values()[i] = EncodeValue::ServoAngle(state.Values[i]);

  return encoded;
}
}  // namespace Zubr