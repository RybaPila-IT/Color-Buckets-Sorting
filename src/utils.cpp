#include "utils.hpp"
#include "constants.hpp"
#include <algorithm>

std::optional<std::fstream> open_file(const std::string& filename){
    std::fstream file;
    file.open(filename, std::ios::app);
    if(!file.good())
        return std::nullopt;
    else
        return file;
}

void print_diagnostics(size_t problemSize, const std::chrono::duration<double, std::milli>& time,
                       size_t numSteps, std::ostream &os){
    os << CSV_SEPARATOR << problemSize << CSV_SEPARATOR << numSteps << CSV_SEPARATOR << time.count() << std::endl;
}

void print_diagnostics(const std::string& algName, const std::vector<char> &problemInstance,
                       const std::vector<char> &result, size_t problemSize,
                       const std::chrono::duration<double, std::milli>& time, size_t numSteps,
                       std::ostream &os) {
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

    os << "Liczba krokow:                " << numSteps << std::endl;

    os << "Czas dzialania algorytmu:    " << time.count() << "ms" << std::endl << std::endl;

}

void print_vector(const std::vector<char> &data, std::ostream &os){
    for(auto &elem : data)
        os << elem << ", ";
    os << std::endl;
}

size_t max_unsorted_length(const std::vector<char> &dataVector){
    size_t CMYKCount[4] = {0};
    for(auto &color : dataVector){
        switch(color){
            case 'C':
                ++CMYKCount[0];
                break;
            case 'M':
                ++CMYKCount[1];
                break;
            case 'Y':
                ++CMYKCount[2];
                break;
            case 'K':
                ++CMYKCount[3];
                break;
            default:
                break;
        }
    }
    return dataVector.size() - (*std::min_element(CMYKCount, CMYKCount + 4) * 4);
}

bool check_string(const std::string &data){
    for(auto &color : data){
        if(std::find(PATTERN, PATTERN + 4, color) == std::end(PATTERN))
            return true;
    }
    return false;
}