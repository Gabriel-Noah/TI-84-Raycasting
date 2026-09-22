'''
Look up table Generator
Generates the look up tables used for sine and cosine
'''

from math import cos, sin, modf, pi

with open("file", "a") as file:
    for i in range(360):
        # file.write(str(sin(i*pi/180)) + ",")
        # number = cos(i * pi / 180)
        number = sin(i * pi / 180)
        number /= 10  # 10 is Ray cast precision

        fPart, iPart = modf(number)

        fixed = int(iPart) << 16  # 16 is SHIFT
        fixed += (int(fPart * 10000) << 16) // 10000

        file.write(str(fixed) + ",")