import subprocess
import textwrap
from os import listdir
from os import path
from os.path import isfile, join, isdir, exists

tester_ver = 1
ASCII_ERROR = """
______  _______  _______  _______  _______ 
(  ____ \(  ____ )(  ____ )(  ___  )(  ____ )
| (    \/| (    )|| (    )|| (   ) || (    )|
| (__    | (____)|| (____)|| |   | || (____)|
|  __)   |     __)|     __)| |   | ||     __)
| (      | (\ (   | (\ (   | |   | || (\ (   
| (____/\| ) \ \__| ) \ \__| (___) || ) \ \__
(_______/|/   \__/|/   \__/(_______)|/   \__/
"""
ASCII_PASSES = """
  , ; ,   .-'^^^'-.   , ; ,
  \\|/  .'         '.  \|//
   \-;-/   ()   ()   \-;-/
   // ;               ; \\
  //__; :.         .; ;__\\
 `-----\'.'-.....-'.'/-----'
        '.'.-.-,_.'.'
          '(  (..-'
            '-'
"""

# paths to files and folder

name_of_good = "good_input"
name_of_invalid = "invalid_input"


path_to_test_files = path.join("tester_files")
path_to_good = path.join(path_to_test_files, name_of_good)
path_to_invalid = path.join(path_to_test_files, name_of_invalid)

path_to_system_out = path.join(path_to_test_files, "system_out")
path_to_user_out = path.join(path_to_test_files, "user_out")

path_to_compiled_files = path.join("cpp_ex2.exe")

# name of files
name_of_user_output_file_no_folder = "_user" + "_output" + ".txt"
name_of_user_errors_file_no_folder = "_user" + "_errors" + ".txt"

name_of_school_solution_output_no_folder = "_school_solution" + "_output" + ".txt"
name_of_school_solution_errors_no_folder = "_school_solution" + "_errors" + ".txt"

invalid_file = "invalid.txt"
num_of_parm_file = "num_of_parm.txt"




num_of_parm = {
    "les_parm": "",
    "more_parm": path.join(path_to_good, "a.txt") + " 1 2 3",
    "no_file": "1 3"
}




def can_not_test(msg):
    print("Error:", msg)
    input("press Enter to exit")
    exit(1)


def run_with_cmd(command_list, str=""):
    """
    Execute the given command list with the command line
    Return a tuple containing the return code, output and errors.
    """

    process = subprocess.Popen(command_list, shell=True, stdin=subprocess.PIPE,
                               stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                               universal_newlines=True)

    process.communicate(str)
    output, errors = process.communicate()
    return process.returncode, output, errors


def print_with_indentation(title, to_print):
    """print text in a nice way"""
    prefix = title + ": "
    wrapper = textwrap.TextWrapper(initial_indent=prefix,
                                   break_long_words=False,
                                   subsequent_indent=' ' * len(prefix))
    print(wrapper.fill(to_print))


def compile_file():
    """
    compile the java files. the compiled files are located in \test_files\compiled_files.
    terminate the tester if there was an error.
    :return: true if was successful.
    """

    command_list = ["make"]

    code, output, errors = run_with_cmd(command_list)
    print(output, errors)
    if code != 0:
        can_not_test("problem with compiling\n" + "error message\n" + errors)
    print("compile OK\n\n")


def run_one_invalid_test(name_of_test, parm, error):
    name_of_school_solution_output = "empty.txt"
    name_of_school_solution_errors = error

    path_to_school_solution_output = path.join(path_to_system_out,
                                               name_of_school_solution_output)
    path_to_school_solution_errors = path.join(path_to_system_out,
                                               name_of_school_solution_errors)

    name_of_user_output = name_of_test + name_of_user_output_file_no_folder
    name_of_user_errors = name_of_test + name_of_user_errors_file_no_folder

    path_to_user_output = path.join(path_to_user_out, name_of_user_output)
    path_to_user_errors = path.join(path_to_user_out, name_of_user_errors)

    print("starting", name_of_test, "..")

    command_list = [path_to_compiled_files] + parm.split()

    code, user_output, user_errors = run_with_cmd(command_list)  # run your code

    # save the files output and errors
    with open(path_to_user_output, 'w') as output_file:
        output_file.write(user_output)

    with open(path_to_user_errors, 'w') as errors_file:
        errors_file.write(user_errors)

    # compare to school solution
    compare_outputs = compare_files(path_to_school_solution_output,
                                    path_to_user_output)
    compare_errors = compare_files(path_to_school_solution_errors,
                                   path_to_user_errors)

    # print helpful information if there was mistakes.
    if compare_outputs is not None or compare_errors is not None:
        # compare failed
        if compare_outputs is not None:
            print("Output file compare failed: here are the details:")
            print_with_indentation("output compare", compare_outputs)
        if compare_errors is not None:
            print("Errors file compare failed: here are the details:")
            print_with_indentation("errors compare", compare_errors)
        return False
    print("passed :)")
    return True


def run_one_good_test(name_of_test, parm):
    """
    run one test. run your code with command file given and with the data given. sva the rusolts in a txt file.
    then compare it to the school solution txt file.
    :param test_folder_name: the name of the folder of the test (tests/test#)
    :param files_to_filter_folder: the data folder  (simple of complex)
    :return: true if was successful.
    """

    name_of_school_solution_output = name_of_test + name_of_school_solution_output_no_folder
    name_of_school_solution_errors = "empty.txt"

    path_to_school_solution_output = path.join(path_to_system_out,
                                               name_of_school_solution_output)
    path_to_school_solution_errors = path.join(path_to_system_out,
                                               name_of_school_solution_errors)

    name_of_user_output = name_of_test + name_of_user_output_file_no_folder
    name_of_user_errors = name_of_test + name_of_user_errors_file_no_folder

    path_to_user_output = path.join(path_to_user_out, name_of_user_output)
    path_to_user_errors = path.join(path_to_user_out, name_of_user_errors)

    print("starting", name_of_test, "..")

    command_list = [path_to_compiled_files] + parm.split()

    code, user_output, user_errors = run_with_cmd(command_list)  # run your code

    # save the files output and errors
    with open(path_to_user_output, 'w') as output_file:
        output_file.write(user_output)

    with open(path_to_user_errors, 'w') as errors_file:
        errors_file.write(user_errors)

    # compare to school solution
    compare_outputs = compare_files(path_to_school_solution_output,
                                    path_to_user_output)
    compare_errors = compare_files(path_to_school_solution_errors,
                                   path_to_user_errors)

    # print helpful information if there was mistakes.
    if compare_outputs is not None or compare_errors is not None:
        # compare failed
        if compare_outputs is not None:
            print("Output file compare failed: here are the details:")
            print_with_indentation("output compare", compare_outputs)
        if compare_errors is not None:
            print("Errors file compare failed: here are the details:")
            print_with_indentation("errors compare", compare_errors)
        return False
    print("passed :)")
    return True


def compare_files(file1, file2):
    """
    compare to files with FC (windows file comparer)
    :param file1:
    :param file2:
    :return: the compaction text if there was errors
    """
    command_to_compare = ['fc', '/W', '/N', '/A', file1, file2]
    code, output, errors = run_with_cmd(command_to_compare)

    if code != 0:  # if code != 0
        print(errors)
        return output
    return None


def run_tests():
    """
    run all the test in the folder test_files\tests with both data folders (simple, complex)
    :return: true iff all passed
    """
    all_passed = True
    tests_invalid = [t for t in listdir(path_to_invalid)]
    tests_good = [t for t in listdir(path_to_good)]
    tests_invalid += ["no_file.csv"]

    number_of_tests = len(num_of_parm) + len(
        tests_invalid) + len(tests_good)

    print("start", number_of_tests, "tests!\nGood luck!\n\n")
    passed_tests = 0

    print(
        "\n********************\nInvalid number of parameters:\n********************\n")
    for name in num_of_parm:  # each test
        if run_one_invalid_test(name, num_of_parm[name], num_of_parm_file):
            passed_tests += 1
        else:
            all_passed = False
        print()

    print(
        "\n********************\nChecking invalid file:\n********************\n")
    for name in tests_invalid:  # each test
        if run_one_invalid_test(name, path.join(path_to_invalid, name), invalid_file):
            passed_tests += 1
        else:
            all_passed = False
        print()


    print(
        "\n********************\nChecking good file:\n********************\n")
    for name in tests_good:  # each test
        if run_one_good_test(name, path.join(path_to_good, name)):
            passed_tests += 1
        else:
            all_passed = False
        print()


    print("\n********************")

    if all_passed:
        print("All tests passed!!")
        return True
    else:
        print("passes", passed_tests, "out of", number_of_tests, "tests")
        return False


def passed_all():
    print(ASCII_PASSES)
    print("you passed everything!!! \ngo get some sleep")
    # startfile(name_of_p_file)


if __name__ == "__main__":
    while True:
        print("starting tester for cpp_ex2 version", tester_ver, '\n')
        compile_file()
        tests_passed = run_tests()

        if tests_passed:
            passed_all()
        else:
            print(ASCII_ERROR)

        input("press enter to restart the tester")
        print('\n\n\nRestarting...')
