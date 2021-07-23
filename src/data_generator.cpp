#include <algorithm>
#include <iostream>

#include "data_generator.hpp"

DataGenerator::DataGenerator(): random_device() {}

DataGenerator::cmyk_arr DataGenerator::generate_colors_probabilities() {
    double c_prob, m_prob, y_prob, k_prob;
    std::mt19937_64 random_engine(random_device());
    std::uniform_real_distribution<> prob_gen(0.0, 1.0);

    c_prob = prob_gen(random_engine);
    m_prob = prob_gen(random_engine);
    y_prob = prob_gen(random_engine);
    k_prob = prob_gen(random_engine);

    double normalization_term = c_prob + m_prob + y_prob + k_prob;

    return {
            std::make_pair(c_prob / normalization_term, 'C'),
            std::make_pair(m_prob / normalization_term, 'M'),
            std::make_pair(y_prob / normalization_term, 'Y'),
            std::make_pair(k_prob / normalization_term, 'K')
    };
}

void DataGenerator::generate_collection(size_t length, const cmyk_arr& cmyk_prob, colors& data) {
    double value;
    std::mt19937_64 random_engine(random_device());
    std::uniform_real_distribution<> val_gen(0, 1.0);

    for (size_t i = 0; i < length; ++i) {
        value = val_gen(random_engine);
        if (value < cmyk_prob[0].first)
            data.push_back(cmyk_prob[0].second);
        else if (value < cmyk_prob[1].first + cmyk_prob[0].first)
            data.push_back(cmyk_prob[1].second);
        else if (value < cmyk_prob[2].first + cmyk_prob[1].first + cmyk_prob[0].first)
            data.push_back(cmyk_prob[2].second);
        else
            data.push_back(cmyk_prob[3].second);
    }
}

ParametricGenerator::ParametricGenerator(): DataGenerator(), c_prob(0.25), m_prob(0.25), y_prob(0.25), k_prob(0.25) {}

ParametricGenerator::ParametricGenerator(double c_p, double m_p, double y_p, double k_p): DataGenerator(), c_prob(c_p), m_prob(m_p), y_prob(y_p), k_prob(k_p) {
    if (c_prob + m_prob + y_prob + k_prob >= 1.01) {
        std::cerr << "WARNING::PARAMETRIC_GENERATOR::Invalid probabilities (sum greater than 1); Creating uniform generator." << std::endl;
        c_prob = 0.25;
        m_prob = 0.25;
        y_prob = 0.25;
        k_prob = 0.25;
    }
}

DataGenerator::colors ParametricGenerator::generate(size_t length) {
    cmyk_arr cmyk_prob_arr = {
            std::make_pair(c_prob, 'C'),
            std::make_pair(m_prob, 'M'),
            std::make_pair(y_prob, 'Y'),
            std::make_pair(k_prob, 'K')
    };
    std::sort(cmyk_prob_arr.begin(), cmyk_prob_arr.end(),
              [](std::pair<double, char> a, std::pair<double, char> b){return a.first < b.first;});
    colors dataVector;
    generate_collection(length, cmyk_prob_arr, dataVector);

    return dataVector;
}

SubstringGenerator::SubstringGenerator(double substr_prob, double mean_substr, double stddev_substr):
        substr_prob(std::min(substr_prob, 1.0)), mean_substr(mean_substr), stddev_substr(stddev_substr) {}

DataGenerator::colors SubstringGenerator::generate(size_t length) {
    auto cmyk_prob = generate_colors_probabilities();
    std::sort(cmyk_prob.begin(), cmyk_prob.end(),
              [](std::pair<double, char> a, std::pair<double, char> b){return a.first < b.first;});
    std::mt19937_64 random_engine(random_device());
    std::uniform_real_distribution<> real_uniform(0.0, 1.0);  // Decision whether substring is sorted or not;
    std::uniform_int_distribution<> int_uniform(0.0, 3.0);    // Generating substring starting letter;
    std::normal_distribution<> normal(mean_substr, stddev_substr);  // Generating substring length;
    colors dataVector;

    while (dataVector.size() < length) {
        uint substr_len = std::min((uint) normal(random_engine), (uint) (length - dataVector.size()));
        if (real_uniform(random_engine) < substr_prob) {
            uint offset = int_uniform(random_engine);
            for (uint i = offset; i < offset + substr_len; i++)
                dataVector.push_back(PATTERN[i%4]);
        } else
            generate_collection(substr_len, cmyk_prob, dataVector);
    }

    return dataVector;
}

DataGenerator::colors PermutationGenerator::get_primary_permutation(size_t length) {
    colors sorted_colors(length);
    for (uint i = 0; i < length; i++)
        sorted_colors[i] = PATTERN[i % PATTERN_LEN];

    return sorted_colors;
}

DataGenerator::colors PermutationGenerator::generate(size_t length) {
    auto start_perm = get_primary_permutation(length);
    std::mt19937_64 random_engine(random_device());
    std::shuffle(start_perm.begin(), start_perm.end(), random_engine);

    return start_perm;
}
