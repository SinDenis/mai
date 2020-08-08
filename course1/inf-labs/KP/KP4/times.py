import os


def main():
    for i in range(1, 19):
        print("test {}".format(i))
        os.system("time ./main tests/test{}\n\n".format(i))


main()
