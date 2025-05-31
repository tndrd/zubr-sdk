import matplotlib.pyplot as plt
import numpy as np

XTarget = [[] for _ in range(24)]
XReal = [[] for _ in range(24)]
XVel = [[] for _ in range(24)]

print(XTarget)

with open("log.txt") as f:
  lines = f.readlines()
  assert len(lines) % 3 == 0

  for i in range(len(lines) // 3):
    target = list(map(float, lines[3*i].split()))
    real = list(map(float, lines[3*i + 1].split()))
    vel = list(map(float, lines[3*i + 2].split()))

    for j in range(24):
      XTarget[j].append(target[j] - 2 * np.pi *round(target[j] / (2*np.pi)))
      XReal[j].append(real[j])
      XVel[j].append(vel[j])


T = np.arange(len(XTarget[0])) * 10
  
plt.plot(T, XTarget[4], "k--", label="Целевая позиция")
plt.plot(T, np.abs(XVel[4]), lw = 1, label="Текущая скорость")
plt.plot(T, XReal[4], label="Текущая позиция")

plt.grid()
plt.legend()
plt.xlabel("t, мс")

plt.show()