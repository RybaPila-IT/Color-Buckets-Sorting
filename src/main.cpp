//
// Created by swirta on 13.05.2021.
//
#include "Data_Generator.hpp"
#include <iostream>
#include <algorithm>

#include "robot.h"

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


void no_args_testing() {

    std::vector<char> colors({'C', 'M', 'Y', 'K', 'K', 'Y', 'C'});
    InstructionList list(std::vector<uint>({0, 1}));

    Robot::sort(colors, list);

    for (auto& c : colors)
        std::cout << c << " ";

    std::cout << std::endl;
}


int main(int argc, char * argv[])
{

    no_args_testing();

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
