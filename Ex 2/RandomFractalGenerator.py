#!/usr/bin/env python3

import random
import uuid
import sys


def get_valid_string():
    a = random.randint(1, 3)
    b = random.randint(1, 6)
    return str(a) + ',' + str(b)


def get_invalid_string():
    string = ''
    loop = random.randint(0, 5)
    for i in range(loop):
        string += str(random.randint(-10, 100))
        if random.randint(0, 1):
            string += ','
        elif random.randint(0, 1):
            string += ' '
        elif random.randint(0, 1):
            string += chr(random.randint(1, 120))
    return string


def write_to_file():
    file_name = "input" + str(uuid.uuid4())[:8]
    f = open(file_name, "w+")
    n = random.randint(0, 100)
    bad = random.randint(0, 1)
    func = get_valid_string
    if bad:
        func = get_invalid_string
    for i in range(n - 1):
        if random.randint(0, 1):
            text = func() + '\n'
            f.write(text)
        else:
            f.write(get_valid_string() + '\n')
    f.write(func())
    f.close()
    file = open("randomInputFiles.txt", "a")
    file.write(file_name + "\n")
    file.close()


def main():
    repeat = 1
    if len(sys.argv) > 1:
        repeat = int(sys.argv[1])
    for i in range(repeat):
        write_to_file()


if __name__ == "__main__":
    main()
