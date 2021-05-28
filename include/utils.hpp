//
// Created by swirta on 28.05.2021.
//

#ifndef AAL_CMYK_UTILS_HPP
#define AAL_CMYK_UTILS_HPP

#include <string>
#include <fstream>
#include <chrono>
#include <optional>
#include <vector>

template<typename T>
std::chrono::duration<double, std::milli> get_time(std::chrono::time_point<T> begin,
                                                   std::chrono::time_point<T> end){
    return std::chrono::duration<double, std::milli>(end - begin);
}

std::optional<std::fstream> open_file(const std::string filename);


void print_diagnostics(const std::string algName, const std::vector<char> &problemInstance,
                       const std::vector<char> &result, const size_t problemSize,
                       const std::chrono::duration<double, std::milli> time, const size_t numSteps,
                       std::ostream &os, const int mode);

#endif //AAL_CMYK_UTILS_HPP
