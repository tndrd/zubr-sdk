#pragma once
#include <cmath>
#include <ostream>

#include "zubr-sdk/ZubrRPC.hpp"

namespace Zubr {
struct Converter {
 private:
  static constexpr double QuatNorm = 1 << 14;
  static constexpr double GyroNorm = 1 << 11;
  static constexpr double AcclNorm = 1 << 12;
  static constexpr double ServoNorm = (1 << 14) / (2 * M_PI);

 public:
  using EncodedMsgs = RPC::Messages;

 public:
  struct DecodedMsgs {
    struct IMU {
      struct XYZ {
        double X;
        double Y;
        double Z;
      };

      struct XYZW {
        double X;
        double Y;
        double Z;
        double W;
      };

      XYZW Quat;
      XYZ Gyro;
      XYZ Accl;
    };

    struct State {
      uint8_t Frame;
      std::array<double, RPC::Messages::MotorCount> Values;
    };
  };

 public:
  struct DecodeValue {
    static double ServoAngle(int16_t value);
    static double QuatComponent(int16_t value);
    static double GyroComponent(int16_t value);
    static double AcclComponent(int16_t value);
  };

  struct EncodeValue {
    static int16_t ServoAngle(double rad);
    static int16_t QuatComponent(double value);
    static int16_t GyroComponent(double value);
    static int16_t AcclComponent(double value);
  };

 public:
  static DecodedMsgs::IMU Decode(const EncodedMsgs::IMU& imu);
  static DecodedMsgs::State Decode(const EncodedMsgs::State& state);
  static EncodedMsgs::State Encode(const DecodedMsgs::State& state);
};
}  // namespace Zubr