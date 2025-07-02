#include <sstream>

#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "zubr-sdk/Zubr.hpp"

using Z = Zubr::Zubr;
namespace py = pybind11;

PYBIND11_MODULE(zubrsdk, m) {
  m.doc() =
      "Library for communicating with Zubr Controller.\n"
      "Author: Lekhterev V.V. @tndrd, Starkit 2025";

  // Structures
  py::class_<Z::State>(m, "State")
      .def(py::init<>())
      .def_readwrite("frame", &Z::State::Frame)
      .def_readwrite("values", &Z::State::Values);

  py::class_<Z::IMU::XYZ>(m, "XYZ")
      .def_readwrite("x", &Z::IMU::XYZ::X)
      .def_readwrite("y", &Z::IMU::XYZ::Y)
      .def_readwrite("z", &Z::IMU::XYZ::Z);

  py::class_<Z::IMU::XYZW>(m, "XYZW")
      .def_readwrite("x", &Z::IMU::XYZW::X)
      .def_readwrite("y", &Z::IMU::XYZW::Y)
      .def_readwrite("z", &Z::IMU::XYZW::Z)
      .def_readwrite("w", &Z::IMU::XYZW::W);

  py::class_<Z::IMU>(m, "IMU")
      .def_readwrite("quat", &Z::IMU::Quat)
      .def_readwrite("gyro", &Z::IMU::Gyro)
      .def_readwrite("accl", &Z::IMU::Accl);

  py::class_<Z::ControllerInfo>(m, "ControllerInfo")
      .def_readwrite("controller_name", &Z::ControllerInfo::ControllerName)
      .def_readwrite("fware_version", &Z::ControllerInfo::FwareVersion)
      .def_readwrite("robot_id", &Z::ControllerInfo::RobotID)
      .def_readwrite("robot_version", &Z::ControllerInfo::RobotVersion)
      .def_readwrite("robot_serial", &Z::ControllerInfo::RobotSerial)
      .def_readwrite("flash_size", &Z::ControllerInfo::FlashSize);

  // Main handle
  py::class_<Z>(m, "Zubr")
      .def(py::init<const std::string&>())
      .def("set_position", &Z::SetPosition)
      .def("get_position", &Z::GetPosition)
      .def("get_velocity", &Z::GetVelocity)
      .def("get_imu", &Z::GetIMU)
      .def("start_slot", &Z::StartSlot)
      .def("get_controller_info", &Z::GetControllerInfo);
}