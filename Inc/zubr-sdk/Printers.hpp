#pragma once
#include "zubr-sdk/Converter.hpp"

namespace Zubr {
namespace Printers {

std::ostream& operator<<(std::ostream& os,
                         const Converter::DecodedMsgs::State& state);
std::ostream& operator<<(std::ostream& os,
                         const Converter::DecodedMsgs::IMU& imu);
std::ostream& operator<<(std::ostream& os, const RPC::Messages::IMU& imu);
std::ostream& operator<<(std::ostream& os, const RPC::Messages::State& state);

}  // namespace Printers
};  // namespace Zubr
