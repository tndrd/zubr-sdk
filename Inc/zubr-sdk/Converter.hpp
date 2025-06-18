#pragma once
#include <cmath>
#include <ostream>

#include "zubr-sdk/ZubrRPC.hpp"

namespace Zubr {

// @brief A collection of conversions between protocol and domain-specific
// message representations
struct Converter {
 private:
  struct Constants {
    static constexpr double QuatNorm = 1 << 14;
    static constexpr double GyroNorm = 1 << 10;
    static constexpr double AcclNorm = 1 << 12;
    static constexpr double ServoNorm = (1 << 14) / (2 * M_PI);
    static constexpr int32_t RobotVersionBase = 115;
    static constexpr int32_t RobotSerialBase = 132;
  };

 public:
  using EncodedMsgs = RPC::Messages;

 public:
  struct DecodedMsgs {
    // @brief IMU data container
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

      // Orientation quaternion
      XYZW Quat;

      // Angular speed, ?rad/s?
      XYZ Gyro;

      // Acceleration, yet unknown units
      XYZ Accl;
    };

    // @brief Servo data container.
    // @brief May represent angles or angular speed.
    struct State {
      // Number of frame which the data corresponds to
      uint8_t Frame;

      // Array of motor values (angle or speed).
      std::array<double, RPC::Messages::MotorCount> Values;
    };

    // @brief Controller info container
    struct ControllerInfo {
      std::string ControllerName;
      int16_t FwareVersion;
      int32_t RobotID;
      int32_t RobotVersion;
      int32_t RobotSerial;
      int16_t FlashSize;
    };
  };

 public:
  // @brief Value decoders from protocol to domain-specific representation
  struct DecodeValue {
    static double ServoAngle(int16_t value);
    static double QuatComponent(int16_t value);
    static double GyroComponent(int16_t value);
    static double AcclComponent(int16_t value);
    static std::string ControllerName(RPC::Messages::ContrNameBuf buf);
  };

  // @brief Value encoders from domain-specific to protocol representation
  struct EncodeValue {
    static int16_t ServoAngle(double rad);
    static int16_t QuatComponent(double value);
    static int16_t GyroComponent(double value);
    static int16_t AcclComponent(double value);
  };

 public:
  // Decode IMU message
  static DecodedMsgs::IMU Decode(const EncodedMsgs::IMU& imu);

  // Decode Servo state message
  static DecodedMsgs::State Decode(const EncodedMsgs::State& state);

  // Decode Controller info message
  static DecodedMsgs::ControllerInfo Decode(
      const EncodedMsgs::ControllerInfo& info);

  // Encode Servo state message
  static EncodedMsgs::State Encode(const DecodedMsgs::State& state);
};
}  // namespace Zubr
