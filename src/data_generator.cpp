#include "data_generator.hpp"
#include <algorithm>

void DataGenerator::generate_collection(size_t length, std::array<std::pair<double, char>, 4> cmykProb,
                                                     std::vector<char>& dataVector) {
    std::mt19937_64 randomEngine(randomDevice());
    std::uniform_real_distribution<> valGen(0, 1.0);

    for(size_t i = 0; i < length; ++i){
        double value = valGen(randomEngine);
        valGen.reset();
        if(value < cmykProb[0].first)
            dataVector.push_back(cmykProb[0].second);
        else if(value < cmykProb[1].first + cmykProb[0].first)
            dataVector.push_back(cmykProb[1].second);
        else if(value < cmykProb[2].first + cmykProb[1].first + cmykProb[0].first)
            dataVector.push_back(cmykProb[2].second);
        else
            dataVector.push_back(cmykProb[3].second);
    }
}

std::vector<char> DataGenerator::random_generator(size_t length) {

    auto cmykProb = generate_colors_probabilities();
    std::sort(cmykProb.begin(), cmykProb.end(), [](std::pair<double, char> a, std::pair<double, char> b){return a.first < b.first;});

    std::vector<char> dataVector;
    generate_collection(length, cmykProb, dataVector);

    return dataVector;
}

std::optional<std::vector<char>> DataGenerator::parametric_generator(size_t length, double cProb, double mProb,
                                                                     double yProb, double kProb) {
    if(cProb + mProb + yProb + kProb > 1.000001)
        return std::nullopt;

    std::array<std::pair<double, char>, 4> cmykProb = {
            std::make_pair(cProb, 'C'),
            std::make_pair(mProb, 'M'),
            std::make_pair(yProb, 'Y'),
            std::make_pair(kProb, 'K')
    };
    std::sort(cmykProb.begin(), cmykProb.end(), [](std::pair<double, char> a, std::pair<double, char> b){return a.first < b.first;});

    std::vector<char> dataVector;
    generate_collection(length, cmykProb, dataVector);

    return dataVector;
}

std::vector<char> DataGenerator::substring_generator(size_t length, double substr_prob, double mean_substr, double stddev_substr) {

    substr_prob = std::min(substr_prob, 1.0);

    auto cmykProb = generate_colors_probabilities();
    std::sort(cmykProb.begin(), cmykProb.end(), [](std::pair<double, char> a, std::pair<double, char> b){return a.first < b.first;});
    std::mt19937_64 randomEngine(randomDevice());
    std::uniform_real_distribution<> realUniform(0.0, 1.0);
    std::uniform_int_distribution<> intUniform(0.0, 3.0);
    std::normal_distribution<> normal(mean_substr, stddev_substr);
    std::vector<char> dataVector;

    while (dataVector.size() < length) {

        uint substr_len = std::min((uint) normal(randomEngine), (uint) (length - dataVector.size()));
        bool is_substr = realUniform(randomEngine) < substr_prob;

        if (is_substr) {

            uint offset = intUniform(randomEngine);

            for (uint i = offset; i < offset + substr_len; i++)
                dataVector.push_back(PATTERN[i%4]);

        } else
            generate_collection(substr_len, cmykProb, dataVector);

    }

    return dataVector;
}

std::vector<char> DataGenerator::no_substring_generator(size_t length) {
    double cProb, mProb, yProb, kProb;
    std::mt19937_64 randomEngine(randomDevice());
    std::uniform_real_distribution<> cGen(0, 1.0);
    cProb = 0.0;
    std::uniform_real_distribution<> mGen(0, 1.0 - cProb);
    mProb = mGen(randomEngine);
    std::uniform_real_distribution<> yGen(0, 1.0 - cProb - mProb);
    yProb = yGen(randomEngine);
    kProb = 1.0 - cProb - mProb - yProb;

    std::array<std::pair<double, char>, 4> cmykProb = {
            std::make_pair(cProb, 'C'),
            std::make_pair(mProb, 'M'),
            std::make_pair(yProb, 'Y'),
            std::make_pair(kProb, 'K')
    };
    auto cmykProbTmp = cmykProb;
    std::sort(cmykProbTmp.begin(), cmykProbTmp.end(), [](std::pair<double, char> a, std::pair<double, char> b){return a.first < b.first;});

    std::vector<char> dataVector;
    generate_collection(1, cmykProbTmp, dataVector);

    for(size_t i = 0; i < length - 1; ++i){
        auto it = std::min_element(cmykProb.begin(), cmykProb.end(), [](std::pair<double, char> a, std::pair<double, char> b){return a.first < b.first;});
        switch(dataVector[i]){
            case 'C':
                if(it->second != 'M'){
                    it->first = cmykProb[1].first;
                    cmykProb[1].first = 0.0;
                }
                break;
            case 'M':
                if(it->second != 'Y'){
                    it->first = cmykProb[2].first;
                    cmykProb[2].first = 0.0;
                }
                break;
            case 'Y':
                if(it->second != 'K'){
                    it->first = cmykProb[3].first;
                    cmykProb[3].first = 0.0;
                }
                break;
            case 'K':
                if(it->second != 'C'){
                    it->first = cmykProb[0].first;
                    cmykProb[0].first = 0.0;
                }
                break;
        }
        cmykProbTmp = cmykProb;
        std::sort(cmykProbTmp.begin(), cmykProbTmp.end(), [](std::pair<double, char> a, std::pair<double, char> b){return a.first < b.first;});
        generate_collection(1, cmykProbTmp, dataVector);
    }
    return dataVector;
}

std::vector<char> DataGenerator::permutation_generator(size_t length, size_t changes) {
    std::vector<char> dataVector = substring_generator(length, 1.0, 1.0, 0.0);
    std::mt19937_64 randomEngine(randomDevice());
    std::uniform_int_distribution<size_t> indexGen(0, length - 4);

    for(size_t i = 0; i < changes; ++i){
        size_t index;
        index = indexGen(randomEngine);
        indexGen.reset();

        for(int j = 0; j < 4; ++j)
            dataVector.insert(dataVector.begin() + index + j, dataVector[dataVector.size() - 4 + j]);

        for(int j = 0; j < 4; ++j)
            dataVector.pop_back();
    }
    return dataVector;
}

std::array<std::pair<double, char>, 4> DataGenerator::generate_colors_probabilities() {
    double cProb, mProb, yProb, kProb;
    std::mt19937_64 randomEngine(randomDevice());
    std::uniform_real_distribution<> probabilityGenerator(0.0, 1.0);

    cProb = probabilityGenerator(randomEngine);
    mProb = probabilityGenerator(randomEngine);
    yProb = probabilityGenerator(randomEngine);
    kProb = probabilityGenerator(randomEngine);

    double normalizationTerm = cProb + mProb + yProb + kProb;

    std::array<std::pair<double, char>, 4> cmykProb = {
            std::make_pair(cProb / normalizationTerm, 'C'),
            std::make_pair(mProb / normalizationTerm, 'M'),
            std::make_pair(yProb / normalizationTerm, 'Y'),
            std::make_pair(kProb / normalizationTerm, 'K')
    };

    return cmykProb;
}
