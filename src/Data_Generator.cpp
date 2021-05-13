//
// Created by swirta on 13.05.2021.
//
#include "Data_Generator.hpp"
#include <algorithm>

void DataGenerator::generate_collection(size_t length, std::array<std::pair<double, char>, 4> cmykProb,
                                                     std::vector<char>& dataVector) {
    std::mt19937_64 randomEngine(randomDevice());
    std::uniform_real_distribution<> valGen(0, 1.0);

    for(size_t i = 0; i < length; ++i){
        double value = valGen(randomEngine);
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
    double cProb, mProb, yProb, kProb;
    std::mt19937_64 randomEngine(randomDevice());

    std::uniform_real_distribution<> cGen(0.0, 1.0);
    cProb = cGen(randomEngine);
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

std::vector<char> DataGenerator::substring_generator(size_t length, double probability) {
    if(probability > 1.0)
        probability = 1.0;

    double cProb, mProb, yProb, kProb;
    std::mt19937_64 randomEngine(randomDevice());
    cProb = probability;
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
    generate_collection(1, cmykProb, dataVector);

    for(size_t i = 1; i < length; ++i){
        switch(dataVector[i - 1]){
            case 'C':
                cmykProb[0].first = cmykProb[1].first;
                cmykProb[1].first = probability;
                break;
            case 'M':
                cmykProb[1].first = cmykProb[2].first;
                cmykProb[2].first = probability;
                break;
            case 'Y':
                cmykProb[2].first = cmykProb[3].first;
                cmykProb[3].first = probability;
                break;
            case 'K':
                cmykProb[3].first = cmykProb[0].first;
                cmykProb[0].first = probability;
                break;
        }
        cmykProbTmp = cmykProb;
        std::sort(cmykProbTmp.begin(), cmykProbTmp.end(), [](std::pair<double, char> a, std::pair<double, char> b){return a.first < b.first;});
        generate_collection(1, cmykProbTmp, dataVector);
    }
    return dataVector;
}

std::vector<char> DataGenerator::no_substring_generator(size_t length) {
    double cProb, mProb, yProb, kProb;
    std::mt19937_64 randomEngine(randomDevice());
    std::uniform_real_distribution<> cGen(0.0, 1.0);
    cProb = cGen(randomEngine);
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
    generate_collection(1, cmykProb, dataVector);

    for(size_t i = 1; i < length; ++i){
        switch(dataVector[i - 1]){
            case 'C':
                cmykProbTmp[0].first = cmykProb[0].first;
                cmykProbTmp[1].first = 0.0;
                break;
            case 'M':
                cmykProbTmp[1].first = cmykProb[1].first;
                cmykProbTmp[2].first = 0.0;
                break;
            case 'Y':
                cmykProbTmp[2].first = cmykProb[2].first;
                cmykProbTmp[3].first = 0.0;
                break;
            case 'K':
                cmykProbTmp[3].first = cmykProb[3].first;
                cmykProbTmp[0].first = 0.0;
                break;
        }
        std::sort(cmykProbTmp.begin(), cmykProbTmp.end(), [](std::pair<double, char> a, std::pair<double, char> b){return a.first < b.first;});
        generate_collection(1, cmykProbTmp, dataVector);
    }
    return dataVector;
}
