import os

test_dir = "test_maps/"
value = 3.4
files = os.listdir(test_dir)

for file in files:
    os.system("./sparse_matrix {} {}".format(test_dir + file, value))
