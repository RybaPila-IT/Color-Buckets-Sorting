//
// Created by swirta on 13.05.2021.
//
#include "Data_Generator.hpp"
#include <iostream>

int main(){
    DataGenerator dataGenerator;
    auto v = dataGenerator.substring_generator(25, 1.0);
    for(auto x : v){
        std::cout << x << ", ";
    }
    std::cout << std::endl;
    return 0;
}
