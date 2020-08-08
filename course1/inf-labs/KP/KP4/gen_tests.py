from random import randint

def main():
    with open('tests/test18', 'w') as f:
        for i in range(8000000):
            f.write("{}\na\n\n".format(randint(1, 10000000)))


main()