#!/usr/bin/python3

import os
import sys
from typing import List
from multiprocessing import Process

#mnijeszy krok dla algorytmu brutalnego
bruteStep = 1
#mniejsza liczba iteracji dla algorytmu brutalnego
bruteIterations = 6
#początkowy rozmiar problemu dla algorytmu brutalnego
initBruteProblemSize = 10

#krok
step = 500
#liczba iteracji
iterations = 100
#poczatkowy rozmiar problemu
initProblemSize = 500
#typy generatorów 
generators = ["-gp", "-gs", "-gns"]
#folder 
folderName = "test_results"


def test_universal_sort(executable : str, generator : str):
    filename = "universal_sort_" + "generator:%s_step:%d_iterations:%d.txt"%(generator, step, iterations)
    filePath = folderName + '/' + filename
    params = "-t %s %d %d %d -us -f %s"%(generator, initProblemSize ,step, iterations, filePath)
    exe = executable + ' ' + params
    
    os.system(exe)

def test_substring_sort(executable : str, generator : str):
    filename = "substring_sort_" + "generator:%s_step:%d_iterations:%d.txt"%(generator, step, iterations)
    filePath = folderName + '/' + filename
    params = "-t %s %d %d %d -ss -f %s"%(generator, initProblemSize ,step, iterations, filePath)
    exe = executable + ' ' + params
    
    os.system(exe)

def test_brute_sort(executable : str, generator : str):
    filename = "brute_force_sort_" + "generator:%s_step:%d_iterations:%d.txt"%(generator, bruteStep, bruteIterations)
    filePath = folderName + '/' + filename
    params = "-t %s %d %d %d -bs -f %s"%(generator, initBruteProblemSize ,bruteStep, bruteIterations, filePath)
    exe = executable + ' ' + params
    
    os.system(exe)

def main():
    try:
        os.mkdir(folderName)
    except FileExistsError:
        pass

    if sys.platform.startswith("win"):
        executable = "powershell.exe ./bin/aal_cmyk "
    else:
        executable = "./bin/aal_cmyk"

    processes : List[Process] = []

    for generator in generators:
        p = Process(target=test_universal_sort, args=(executable, generator))
        processes.append(p)

    for generator in generators:
        p = Process(target=test_substring_sort, args=(executable, generator))
        processes.append(p)

    for generator in generators:
        p = Process(target=test_brute_sort, args=(executable, generator))
        processes.append(p)

    for process in processes:
        process.start()
    
    for process in processes:
        process.join()

if __name__ == "__main__":
    main()