#!/usr/bin/env python3

import os
import sys
import subprocess


def getInputFiles():
    if len(sys.argv) == 1 or sys.argv[-1][-3:] == 'cpp':
        raise Exception("Please insert the name of your COMPILED file.")
    if len(sys.argv) < 3:
        os.system("RandomFractalGenerator.py")
        f = "randomInputFiles.txt"
        name = sys.argv[1]
    else:
        f = sys.argv[1]
        name = sys.argv[2]
    with open(f) as file:
        return file.readlines(), name


def runTests():
    lines, name  = getInputFiles()
    for line in lines:
        if len(line) > 1:
            print(line)
            file_name = line[:-1]
            cmnd1 = "~proglab/www/cpp_ex2/FractalDrawer " + file_name + " >& school_out_" + file_name
            os.system(cmnd1)
            os.system(name + " " + file_name + " >& my_out_" + file_name)
            # os.system("diff -q -s school_out_"+inp[0] + " my_out_"+ inp[0])
            # with open("my_out_"+line[0:-1]) as file:
            #    my = file.readlines()
            #    with open("school_out_"+line[0:-1]) as file2:
            #        school = file2.readlines()
            #        file.close()
            #        file2.close()
            #        print(my)
            #        print(school)
            try:
                output = subprocess.check_output(
                    ["diff", "-q", "-s", "school_out_" + file_name, "my_out_" + file_name])
                if "identical" in str(output):
                    print("Passed!")
            except subprocess.CalledProcessError:
                print("Failed :("
                      "######################################################"
                      "######################################################"
                      "###########")
                # os.system("diff school_out_"+line[0:-1] + "my_out_"+line[0:-1])
            print()
            print()


runTests()
