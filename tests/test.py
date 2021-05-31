import os
import sys

def run_tests():
    try:
        os.mkdir("test_results")
    except FileExistsError:
        pass

    if sys.platform.startswith("win"):
        executable = "powershell.exe ./bin/aal_cmyk"
    else:
        executable = "./bin/aal_cmyk"
    
    universalFile = " -f test_results/universal_sort_parametric.txt"
    substringFile = " -f test_results/substring_sort_parametric.txt"
    bruteFile = " -f test_results/brute_sort_parametric.txt"

    os.system(executable + " -t -gp 10 3 2 -us" + universalFile)
    os.system(executable + " -t -gp 10 3 2 -ss" + substringFile)
    os.system(executable + " -t -gp 10 3 2 -bs" + bruteFile)

if __name__ == "__main__":
    run_tests()