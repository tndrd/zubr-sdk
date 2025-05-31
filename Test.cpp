#include "zubr-sdk/Zubr.hpp"
#include <Printers.hpp>
#include <cassert>
#include <iostream>
#include <unistd.h>

using namespace Zubr::Printers;

int main(int argc, char** argv) try {
  Zubr::Zubr zubr ("/dev/ttyACM0");

  auto imu = zubr.GetIMU();
  auto position = zubr.GetPosition();
  auto velocity = zubr.GetVelocity();

  double x = 0;

  while(x < 15) {
    position.Frame++;
    position.Values[4] = fabs(sin(x));
    x+= .01;
    zubr.SetPosition(position);
    
    auto newPos = zubr.GetPosition();
    auto vel = zubr.GetVelocity();

    for (int i = 0; i < position.Values.size(); ++i) {
      std::cout << position.Values[i] << " ";
    }
    std::cout << std::endl;

    for (int i = 0; i < newPos.Values.size(); ++i) {
      std::cout << newPos.Values[i] << " ";
    }
    std::cout << std::endl;

    for (int i = 0; i < vel.Values.size(); ++i) {
      std::cout << vel.Values[i] << " ";
    }
    std::cout << std::endl;


    //std::cout << zubr.GetPosition() << std::endl;
    //std::cout << zubr.GetVelocity() << std::endl;
    usleep(1000*10);
  }

  return 0;
} catch (std::exception& e) {
  std::cout << "Exception: " << e.what() << std::endl;
}