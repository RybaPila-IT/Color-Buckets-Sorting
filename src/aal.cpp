//
// Created by swirta on 13.05.2021.
//
#include "data_generator.hpp"
#include "alg_sort.hpp"
#include "robot.h"
#include <iostream>
#include <algorithm>

using namespace std;

char* getCmdOption(char ** begin, char ** end, const std::string & option)
{
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return nullptr;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}

int main(int argc, char * argv[])
{
    if(argc < 2){
        cout << "Too few arguments" << endl;
        return 0;
    }
    /*
     * interactive mode
     * ./aal_cmyk -i
     */
    if(cmdOptionExists(argv, argv + argc, "-i"))
    {
        cout << "interactive works" << endl;
    }
    /*
    * instance mode
    * ./aal_cmyk -s <rozmiar problemu> <tryb generatora> -f <plik wynikowy>
    */
    else if(cmdOptionExists(argv, argv + argc, "-s"))
    {
        auto problemSize_ = argv[2];
        auto generatorMode_ = argv[3];
        char *filename_ = getCmdOption(argv, argv + argc, "-f");

        int problemSize = atoi(problemSize_);
        if(problemSize < 0){
            cout << "Invalid problem size: " << problemSize << endl;
            return 0;
        }

        //wywołanie generatora
        //wywołanie algorytmu

        if(filename_){
            //Zapis do pliku
        }
        else{
            cout << "Output file was not specified" << endl;
        }
    }
    /*
    * testing mode
    * ./aal_cmyk -t <rozmiar problemu> <krok> <liczba iteracji> <tryb generatora> -f <plik wynikowy>
    */
    else if(cmdOptionExists(argv, argv + argc, "-t"))
    {
        auto problemSize_ = argv[2];
        auto step_ = argv[3];
        auto iterations_ = argv[4];
        auto generatorMode_ = argv[5];
        char *filename_ = getCmdOption(argv, argv + argc, "-f");

        int problemSize = atoi(problemSize_);
        int step = atoi(step_);
        int iterations = atoi(iterations_);

        if(problemSize < 0){
            cout << "Invalid problem size: " << problemSize << endl;
            return 0;
        }
        if(step < 0){
            cout << "Invalid step: " << step << endl;
            return 0;
        }
        if(iterations < 0){
            cout << "Invalid iterations: " << iterations << endl;
            return 0;
        }

        if(filename_){
            //otwarcie pliku
        }
        else{
            cout << "Output file was not specified" << endl;
            return 0;
        }

        for(int i = 0; i < iterations; ++i){
            //wywołanie generatora
            //wywołanie algorytmu
            //zapis do pliku
        }
    }
    else if(cmdOptionExists(argv, argv + argc, "-h"))
    {
        cout << "Interactive mode:  " << "./aal_cmyk -i" << endl;
        cout << "Instance mode:     " << "./aal_cmyk -s <problem size> <generator mode> -f <output file>" << endl;
        cout << "Testing mode:      " << "./aal_cmyk -t <problem size> <step> <number of iterations> <generator mode> -f <output file>" << endl;
    }
    return 0;
}