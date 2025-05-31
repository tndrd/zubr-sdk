#include "zubr-sdk/Printers.hpp"

static constexpr size_t NDigits = 2;

template <typename T>
T PRound(T value) {
  double norm = pow(10, NDigits);
  return round(value * norm) / norm;
}

namespace Zubr {
namespace Printers {

std::ostream& operator<<(std::ostream& os,
                         const Converter::DecodedMsgs::State& state) {
  os << "Frame: " << +state.Frame << std::endl;
  for (int i = 0; i < state.Values.size(); ++i)
    os << " #" << i << ": " << (i <= 9 ? " " : "") << PRound(state.Values[i])
       << std::endl;
  return os;
}

std::ostream& operator<<(std::ostream& os,
                         const Converter::DecodedMsgs::IMU& imu) {
  auto dumpXYZ = [&](const char* name, Converter::DecodedMsgs::IMU::XYZ xyz) {
    os << " " << name << ": {" << PRound(xyz.X) << ", " << PRound(xyz.Y) << ", "
       << PRound(xyz.Z) << "}" << std::endl;
  };

  auto dumpXYZW = [&](const char* name,
                      Converter::DecodedMsgs::IMU::XYZW xyzw) {
    os << " " << name << ": {" << PRound(xyzw.X) << ", " << PRound(xyzw.Y)
       << ", " << PRound(xyzw.Z) << ", " << PRound(xyzw.W) << "}" << std::endl;
  };

  os << "IMU,   {x, y, z, [w]}: " << std::endl;
  dumpXYZW("Quat", imu.Quat);
  dumpXYZ("Gyro", imu.Gyro);
  dumpXYZ("Accl", imu.Accl);

  return os;
}

std::ostream& operator<<(std::ostream& os, const RPC::Messages::IMU& imu) {
  return os << Converter::Decode(imu);
}

std::ostream& operator<<(std::ostream& os, const RPC::Messages::State& state) {
  return os << Converter::Decode(state);
}

}  // namespace Printers
};  // namespace Zubr
