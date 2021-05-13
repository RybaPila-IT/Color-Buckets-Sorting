//
// Created by swirta on 13.05.2021.
//

#ifndef AAL_ALGORYTM_SORTOWANIA_DATA_GENERATOR_HPP
#define AAL_ALGORYTM_SORTOWANIA_DATA_GENERATOR_HPP

#include <chrono>
#include <random>
#include <vector>
#include <array>
#include <optional>

const char CMYK[4] = {'C', 'M', 'Y', 'K'};

class DataGenerator{
public:
    std::vector<char> random_generator(size_t length);
    std::optional<std::vector<char>> parametric_generator(size_t length, double cProb, double mProb, double yProb, double kProb);
    std::vector<char> substring_generator(size_t length, double probability);
    std::vector<char> no_substring_generator(size_t length);

private:
    void generate_collection(size_t length, std::array<std::pair<double, char>, 4> cmykProb, std::vector<char>& dataVector);
    std::random_device randomDevice;
};

#endif //AAL_ALGORYTM_SORTOWANIA_DATA_GENERATOR_HPP
