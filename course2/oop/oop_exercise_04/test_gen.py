#!/usr/bin/env python3

import random
import math
import sys


def rhombus_gen(rhombus_num):
    for i in range(rhombus_num):
        cor_type = random.randint(0, 1)
        if cor_type == 1:
            x0 = random.randint(-1000, 1000)
            y0 = random.randint(-1000, 1000)
            a = random.randint(10, 100)
            print('RI')
            print(x0, y0)
            print(x0, y0 + a)
            print(x0 + a, y0 + a)
            print(x0 + a, y0)
        else:
            x0 = random.random() * 2000 - 1000
            y0 = random.random() * 2000 - 1000
            a = random.random() * 90 + 10
            print('RD')
            print(x0, y0)
            print(x0, y0 + a)
            print(x0 + a, y0 + a)
            print(x0 + a, y0)


def pentagon_gen(pentagons_num):
    for i in range(pentagons_num):
        cor_type = random.randint(0, 1)
        if cor_type == 1:
            x0 = random.randint(-1000, 1000)
            y0 = random.randint(-1000, 1000)
            r = random.randint(10, 100)
            print('PI')
            for j in range(5):
                print(int(x0 + r * math.cos(2 * math.pi * j / 5)), int(y0 + r * math.sin(2 * math.pi * j / 5)))
        else:
            x0 = random.random() * 2000 - 1000
            y0 = random.random() * 2000 - 1000
            r = random.random() * 90 + 10
            print('PD')
            for j in range(5):
                print(x0 + r * math.cos(2 * math.pi * j / 5), y0 + r * math.sin(2 * math.pi * j / 5))


def hexagon_gen(hexagons_num):
    for i in range(hexagons_num):
        cor_type = random.randint(0, 1)
        if cor_type == 1:
            x0 = random.randint(-1000, 1000)
            y0 = random.randint(-1000, 1000)
            r = random.randint(10, 100)
            print('HI')
            for j in range(6):
                print(int(x0 + r * math.cos(2 * math.pi * j / 6)), int(y0 + r * math.sin(2 * math.pi * j / 6)))
        else:
            x0 = random.random() * 2000 - 1000
            y0 = random.random() * 2000 - 1000
            r = random.random() * 90 + 10
            print('HD')
            for j in range(6):
                print(x0 + r * math.cos(2 * math.pi * j / 6), y0 + r * math.sin(2 * math.pi * j / 6))


def main():
    rhombus_num = int(sys.argv[1])
    pentagons_num = int(sys.argv[2])
    hexagons_num = int(sys.argv[3])
    rhombus_gen(rhombus_num)
    pentagon_gen(pentagons_num)
    hexagon_gen(hexagons_num)


main()
