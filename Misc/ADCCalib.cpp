#include <unistd.h>

#include <cassert>
#include <iostream>

#include "zubr-sdk/Zubr.hpp"

using namespace Zubr::Printers;

int main(int argc, char** argv) try {
  Zubr::Zubr zubr("/dev/ttyACM0");

  while (true) {
    double batLevel = zubr.GetBatteryLevel();
    std::cout << "Bat level: " << batLevel << std::endl;
    usleep(100 * 1000);
  }
  return 0;
} catch (std::exception& e) {
  std::cout << "Exception: " << e.what() << std::endl;
}