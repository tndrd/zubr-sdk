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