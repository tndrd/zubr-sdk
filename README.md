# Zubr-SDK

C++ library to communicate with "Zubr" servo controller via USB

# How to build

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

# Usage

Minimal example:

```cpp
#include<zubr-sdk/Zubr.hpp>

// Enable << overloads
using namespace Zubr::Printers;

int main() {
  // Dynamic port configuration is
  // yet to be implemented
  Zubr::Zubr zubr ("/dev/ttyACM0");

  auto imu = zubr.GetIMU();
  auto position = zubr.GetPosition();
  auto velocity = zubr.GetVelocity();

  std::cout << imu << std::endl;
  std::cout << position << std::endl;
  std::cout << velocity << std::endl;

  position.Frame += 1;

  for(int i = 0; i < position.Values.size(); ++i)
    position.Values[i] = 0;

  auto newPosition = zubr.SetPosition(position);

  std::cout << newPosition << std::endl;
  return 0;
}

```
