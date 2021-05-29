#ifndef AAL_CMYK_DATA_GENERATOR_HPP
#define AAL_CMYK_DATA_GENERATOR_HPP

#include <chrono>
#include <random>
#include <vector>
#include <array>
#include <optional>
#include "constants.hpp"

class DataGenerator {

protected:
    using cmyk_arr = std::array<std::pair<double, char>, 4>;
    using colors   = std::vector<char>;

    std::random_device random_device;

    cmyk_arr generate_colors_probabilities();
    void     generate_collection(size_t length, const cmyk_arr& cmyk_prob, std::vector<char>& data);

public:
    DataGenerator();

    virtual colors generate(size_t length) = 0;
};

class ParametricGenerator: public DataGenerator {

private:
    double c_prob;
    double m_prob;
    double y_prob;
    double k_prob;

public:
    ParametricGenerator();
    ParametricGenerator(double c_p, double m_p, double y_p, double k_p);

    colors generate(size_t length) override;
};


class SubstringGenerator: public DataGenerator {

private:
    double substr_prob;
    double mean_substr;
    double stddev_substr;

public:
    SubstringGenerator(double substr_prob, double mean_substr, double stddev_substr);

    colors generate(size_t length) override;
};


class PermutationGenerator: public DataGenerator {

private:
    static colors get_primary_permutation(size_t length);

public:
    colors generate(size_t length) override;
};


#endif //AAL_CMYK_DATA_GENERATOR_HPP
