from zubrsdk import Zubr, State

zubr = Zubr("/dev/ttyACM0")

state = State()
state.frame = 0

# len(state.values) всегда равен числу моторов (25)
for i in range(len(state.values)):
  state.values[i] = 0

pos = zubr.set_position(state)
print(pos.frame)
print(pos.values)

pos = zubr.get_position()
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