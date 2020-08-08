def main():
    print("Load test1.tr")
    print("Print")
    for i in range(100):
        print("Add H {} {} {} {}".format(i, i - 32, i * 2 - 3, i + 90))
    for i in range(30):
        print("Undo")



main()
