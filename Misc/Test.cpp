#include "zubr-sdk/Zubr.hpp"
#include <cassert>
#include <iostream>
#include <unistd.h>

using namespace Zubr::Printers;

int main(int argc, char** argv) try {
  Zubr::Zubr zubr ("/dev/ttyACM0");

  auto pos = zubr.SetPosition({});
  std::cout << pos << std::endl;
  
  pos = zubr.GetPosition();
  std::cout << pos << std::endl;

  auto vel = zubr.GetVelocity();
  std::cout << vel << std::endl;

  auto IMU = zubr.GetIMU();
  std::cout << IMU << std::endl;

  return 0;
} catch (std::exception& e) {
  std::cout << "Exception: " << e.what() << std::endl;
}