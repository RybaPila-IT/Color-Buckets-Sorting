//
// Created by swirta on 28.05.2021.
//
#include "utils.hpp"
#include "constants.hpp"

std::optional<std::fstream> open_file(const std::string filename){
    std::fstream file;
    file.open(filename, std::ios::app);
    if(!file.good())
        return std::nullopt;
    else
        return std::move(file);
}

void print_diagnostics(const std::string algName, const std::vector<char> &problemInstance,
                       const std::vector<char> &result, const size_t problemSize,
                       const std::chrono::duration<double, std::milli> time, const size_t numSteps,
                       std::ostream &os, const int mode){
    switch(mode){
        case 0:
            os << "Nazwa algorytmu:             " << algName << std::endl;

            os << "Instancja problemu:          ";
            for(auto &elem : problemInstance)
                os << elem << ", ";
            os << std::endl;

            os << "Wynik dzialania algorytmu:   ";
            for(auto &elem : result)
                os << elem << ", ";
            os << std::endl;

            os << "Rozmiar problemu:            " << problemSize << std::endl;

            os << "Ilosc krokow:                " << numSteps << std::endl;

            os << "Czas dzialania algorytmu:    " << time.count() << "ms" << std::endl << std::endl;
            break;
        case 1:
            os << algName << CSV_SEPARATOR << problemSize << CSV_SEPARATOR << numSteps << CSV_SEPARATOR << time.count() << std::endl;
            break;
        default:
            break;
    }
}