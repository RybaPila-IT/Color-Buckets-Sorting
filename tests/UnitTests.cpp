//
// Created by swirta on 14.05.2021.
//
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include <iostream>

#include "Data_Generator.hpp"

BOOST_AUTO_TEST_CASE(Data_Generator_Test){
    std::cout << "---DataGenerator test---" << std::endl;

    DataGenerator dataGenerator;
    size_t length = 100;

    std::cout << "---DataGenerator::random_generator---" << std::endl;
    auto dataVector = dataGenerator.random_generator(length);
    BOOST_CHECK_EQUAL(dataVector.size(), length);

    std::cout << "---DataGenerator::parametric_generator---" << std::endl;
    auto value = dataGenerator.parametric_generator(length, 0.2, 0.3, 0.4, 0.11);
    BOOST_CHECK_EQUAL(value.has_value(), false);

    value = dataGenerator.parametric_generator(length, 0.2, 0.3, 0.4, 0.1);
    BOOST_CHECK_EQUAL(value.value().size(), length);

    std::cout << "---DataGenerator::substring_generator---" << std::endl;
    dataVector = dataGenerator.substring_generator(length, 1.1);
    BOOST_REQUIRE_EQUAL(dataVector.size(), length);
    bool flag = false;
    for(size_t i = 1; i < length; ++i){
        switch (dataVector[i - 1]) {
            case 'C':
                if(dataVector[i] != 'M')
                    flag = true;
                break;
            case 'M':
                if(dataVector[i] != 'Y')
                    flag = true;
                break;
            case 'Y':
                if(dataVector[i] != 'K')
                    flag = true;
                break;
            case 'K':
                if(dataVector[i] != 'C')
                    flag = true;
                break;
        }
    }
    BOOST_CHECK_EQUAL(flag, false);

    std::cout << "---DataGenerator::no_substring_generator---" << std::endl;
    dataVector = dataGenerator.no_substring_generator(length);
    BOOST_REQUIRE_EQUAL(dataVector.size(), length);
    flag = false;
    for(size_t i = 1; i < length; ++i){
        switch (dataVector[i - 1]) {
            case 'C':
                if(dataVector[i] == 'M')
                    flag = true;
                break;
            case 'M':
                if(dataVector[i] == 'Y')
                    flag = true;
                break;
            case 'Y':
                if(dataVector[i] == 'K')
                    flag = true;
                break;
            case 'K':
                if(dataVector[i] == 'C')
                    flag = true;
                break;
        }
    }
    BOOST_CHECK_EQUAL(flag, false);
}