#pragma once
#include "zubr-sdk/Converter.hpp"

namespace Zubr {
namespace Printers {

std::ostream& operator<<(std::ostream& os,
                         const Converter::DecodedMsgs::State& state);
std::ostream& operator<<(std::ostream& os,
                         const Converter::DecodedMsgs::IMU& imu);
std::ostream& operator<<(std::ostream& os,
                         const Converter::DecodedMsgs::ControllerInfo& info);

/*
std::ostream& operator<<(std::ostream& os, const RPC::Messages::IMU& imu);
std::ostream& operator<<(std::ostream& os, const RPC::Messages::State& state);
std::ostream& operator<<(std::ostream& os,
                         const RPC::Messages::ControllerInfo& info);
*/

}  // namespace Printers
};  // namespace Zubr
