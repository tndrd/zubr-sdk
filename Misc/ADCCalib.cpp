#include <unistd.h>

#include <cassert>
#include <iostream>

#include "zubr-sdk/Zubr.hpp"

using namespace Zubr::Printers;

int main(int argc, char** argv) try {
  Zubr::Zubr zubr("/dev/ttyACM0");

  // How to set calibration parameters
  // Zubr::Converter::Constants::BattSlope = 420;
  // Zubr::Converter::Constants::BattBias = 69;

  while (true) {
    double batLevel = zubr.GetBatteryLevel();
    int32_t adcValue = zubr.GetBatteryADC();
    std::cout << "Bat level: " << batLevel << " [" << adcValue << "]"
              << std::endl;
    usleep(100 * 1000);
  }
  return 0;
} catch (std::exception& e) {
  std::cout << "Exception: " << e.what() << std::endl;
}