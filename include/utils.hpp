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

std::optional<std::fstream> open_file(const std::string& filename);


void print_diagnostics(const std::string& algName, const std::vector<char> &problemInstance,
                       const std::vector<char> &result, size_t problemSize,
                       const std::chrono::duration<double, std::milli>& time, size_t numSteps,
                       std::ostream &os, int mode);

size_t max_unsorted_length(const std::vector<char> &dataVector);

#endif //AAL_CMYK_UTILS_HPP
