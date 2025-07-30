# Zubr-SDK

C++ library to communicate with "Zubr" servo controller via USB

# How to build

```bash
mkdir build
cd build
cmake ..
cmake --build .

# Install python binding
sudo cmake --install .
```

# Basic Usage
## Minimal example:

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
  auto info = zubr.GetControllerInfo();
  double batLevel = zubr.GetBatteryLevel();
  
  std::cout << imu << std::endl;
  std::cout << position << std::endl;
  std::cout << velocity << std::endl;
  std::cout << info << std::endl;
  std::cout << "Bat level: " << batLevel << std::endl;


  position.Frame += 1;

  for(int i = 0; i < position.Values.size(); ++i)
    position.Values[i] = 0;

  auto newPosition = zubr.SetPosition(position);

  std::cout << newPosition << std::endl;
  return 0;
}

```

## Initialization

To initialize Zubr handle, you first need to include corresponding header.
Optionally, you may enable ```<<``` overloads to natively dump library structures
into ```std::ostream```'s.

```cpp
#include<zubr-sdk/Zubr.hpp>

// Enable << overloads
using namespace Zubr::Printers;
...
```

Next, the handle needs to be initialized. To do this, a port connected to Zubr controller has to be known.
Usually it would be ```/dev/ttyACM0```.

```cpp
...
Zubr::Zubr zubr ("/dev/ttyACM0");
...
```

> Dynamic port obtainment is to be implemented somewhere in future

## Getter procedures

```Zubr``` class incapsulates several read-only calls. Let's look at them

### Controller info obtainment

You can get some controller info by calling ```Zubr::GetControllerInfo```:
```cpp
...
auto info = zubr.GetControllerInfo();
...
```

The info structure has a following layout:

```cpp
struct ControllerInfo {
  std::string ControllerName;
  int16_t FwareVersion;
  int32_t RobotID;
  int32_t RobotVersion;
  int32_t RobotSerial;
  int16_t FlashSize;
};
```

### IMU data obtainment

To get IMU data, ```Zubr::GetIMU``` should be called.

```cpp
...
auto imu = zubr.GetIMU();
...
```

The function returns a ```Zubr::Converter::DecodedMsgs::IMU``` structure, which has a field layout like this:

```cpp
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
```

### Servo position and velocity obtainment

To get positions or velocities of all servos, ```Zubr::GetPosition``` or ```Zubr::GetVelocity``` have to be called.

```cpp
...
auto position = zubr.GetPosition();
auto velocity = zubr.GetVelocity();
...
```

Both of them return a ```Zubr::Converter::DecodedMsgs::State``` structure:
```cpp
struct State {
  // Number of frame which the data corresponds to
  uint8_t Frame;

  // Array of motor values (angle or speed).
  // i-th element corresponds to i-th motor
  std::array<double, RPC::Messages::MotorCount> Values;
};
```

> Notice the ```Frame``` field, it is crucial for ```Zubr::SetPosition``` calls.

Servo positions are returned in radians relative to zero positions set by trimming. Servo velocities are returned in radians per second.

### Battery level obtainment

It is possible to get current battery level in volts using ```zubr::GetBatteryLevel``` method

```cpp
double batLevel = zubr.GetBatteryLevel();
std::cout << "Bat level: " << batLevel << std::endl;
```

## Native print operators

Assuming you've enabled ```<<``` overloads, library structures are able to be dumped in any ```std::ostream```:

```cpp
...
std::cout << imu << std::endl;
std::cout << position << std::endl;
std::cout << velocity << std::endl;
std::cout << info << std::endl;
...
```

## Setter procedures
### Setting servo positions

To set positions, ```Zubr::SetPosition``` function has to be called. But before this, a current **frame number** has to obtained from ```Zubr::GetPosition``` call. New positions should be tied to a frame number higher than the current one. Otherwise, the controller would **discard** your command.

After the frame number has been obtained, a ```Zubr::State``` structure has to be filled with new positions (in radians), and a new frame number.

The return value of ```Zubr::SetPosition``` is similar to ```Zubr::GetPosition```: it returns current servo positions.

```cpp
...
auto currPos = zubr.GetPosition();

Zubr::State newPos;
newPos.Frame = currPos.Frame + 1;

for(int i = 0; i < newPos.Values.size(); ++i)
  newPos.Values[i] = 0;

currPos = zubr.SetPosition(position);
...
```

# Python binding

Library functions are binded to python using ```pybind11```. Usage is very similar to C++ except some syntax differences:

```py
from zubrsdk import Zubr

zubr = Zubr("/dev/ttyACM0")

pos = zubr.get_position()
print(pos.frame)
print(pos.values)

# pos.values ARE NOT MODIFIABLE
# via [] operator (binding speific),
# following code would not work:
#
# for i in range(len(pos.values)):
#  pos.values[i] = 0.137
#
# The pos.values field is to be filled
# using python list assignment, as in
# example:

mcount = len(pos.values)
newpos = [0.0] * mcount

for i in range(len(newpos)):
  newpos[i] = 0.137

pos.values = newpos
pos.frame += 1

pos = zubr.set_position(pos)
print(pos.frame)
print(pos.values)

vel = zubr.get_velocity()
print(vel.frame)
print(vel.values)

imu = zubr.get_imu()
print(imu.quat.x)
print(imu.quat.y)
print(imu.quat.z)
print(imu.quat.w)

print(imu.gyro.x)
print(imu.gyro.y)
print(imu.gyro.z)

print(imu.accl.x)
print(imu.accl.y)
print(imu.accl.z)

info = zubr.get_controller_info()
print(info.controller_name)
```
