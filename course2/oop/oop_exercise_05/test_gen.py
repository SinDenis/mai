#!/usr/bin/env python3
import random
import sys


def rhombus_generate(x, y):
    x_offset = random.randint(10, 100)
    y_offset = random.randint(10, 100)
    print('insert',
          x - x_offset, y,
          x, y + y_offset,
          x + x_offset, y,
          x, y - y_offset)


def main():
    num_test = int(sys.argv[1])
    for i in range(num_test):
        rhombus_generate(i, i + 10)
    for i in range(10):
        print('count', random.randint(1000, 10000))
    num_test //= 2
    print('print')
    for i in range(num_test):
        print('remove', i)
    print('print')


if __name__ == '__main__':
    main()
