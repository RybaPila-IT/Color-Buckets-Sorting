//
// Created by swirta on 13.05.2021.
//
#include "Data_Generator.hpp"
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
    return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}

int main(int argc, char * argv[])
{
    if(argc < 2){
        cout << "Too few arguments" << endl;
    }
    if(cmdOptionExists(argv, argv + argc, "-interactive"))
    {
        std::cout << "interactive works" << std::endl;
    }
    else if(cmdOptionExists(argv, argv + argc, "-instance"))
    {
        std::cout << "instance works" << std::endl;
    }
    else if(cmdOptionExists(argv, argv + argc, "-testing"))
    {
        std::cout << "testing works" << std::endl;
    }
    else if(cmdOptionExists(argv, argv + argc, "-h"))
    {
        std::cout << "h works" << std::endl;
    }

    char * filename = getCmdOption(argv, argv + argc, "-f");

    if (filename)
    {
        // Do interesting things
        // ...
    }

    return 0;
}
