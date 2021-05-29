#include "data_generator.hpp"
#include "alg_sort.hpp"
#include "robot.h"
#include "utils.hpp"
#include <iostream>
#include <chrono>


//char* getCmdOption(char ** begin, char ** end, const std::string & option)
//{
//    char ** itr = std::find(begin, end, option);
//    if (itr != end && ++itr != end)
//    {
//        return *itr;
//    }
//    return nullptr;
//}
//
//bool cmdOptionExists(char** begin, char** end, const std::string& option)
//{
//    return std::find(begin, end, option) != end;
//}

void simulate(std::vector<char>& colors, InstructionList& list) {
  
    std::cout << "List len: " << list.size() << std::endl;
    Robot::sort(colors, list);
}

void no_args_run_uniform() {

    std::cout << "Run uniform sort;" << std::endl;

    //std::vector<char> colors({'C', 'C', 'M','M', 'Y', 'Y', 'K', 'K'});
    //std::vector<char> colors({'C', 'Y', 'Y','M', 'C', 'M', 'Y', 'K'});
    DataGenerator generator;
    std::vector<char> colors;
    //colors = generator.parametric_generator(200, 0.25, 0.25, 0.25, 0.25).value();
    //colors = std::vector<char>({'M', 'C', 'Y', 'K', 'C', 'M', 'Y', 'K'});
    colors = generator.parametric_generator(50, 0.25, 0.25, 0.25, 0.25).value();
    auto original = colors;
    //colors = generator.substring_generator(80 , 1.0, 8, 5);
    auto begin = std::chrono::high_resolution_clock::now();
    auto list = universal_sort(colors);
    auto end = std::chrono::high_resolution_clock::now();

    simulate(colors, list);

    auto time = get_time(begin, end);
    auto file = open_file("test.txt");
    if(file.has_value())
        print_diagnostics("test", original, colors, 50, time, list.size(), file.value(), 1);

    print_diagnostics("test", original, colors, 50, time, list.size(), std::cout, 0);
}

void no_args_run_substring() {

    std::cout << "Run substring sort;" << std::endl;

    DataGenerator generator;

    std::vector<char> colors;

    //colors = std::vector<char>({'C', 'M', 'Y', 'K', 'C', 'M', 'Y', 'K'});
    //colors = std::vector<char>({'M', 'M', 'Y', 'C', 'M', 'Y', 'K', 'C', 'M', 'M', 'C', 'M', 'Y'});
    //colors = std::vector<char>({'K', 'C', 'Y', 'K', 'C', 'M', 'Y', 'M'});
    //colors = std::vector<char>({'K', 'C', 'Y', 'C', 'C', 'M', 'Y', 'K', 'C', 'M', 'Y', 'K', 'Y', 'M', 'C', 'K', 'C', 'M','Y', 'K'});
    colors = generator.substring_generator(80 , 1.0, 8, 5);

    auto list = substrings_sort(colors);

    simulate(colors, list);
}

void no_args_run_brute() {

    std::cout << "Run brute-force sort" << std::endl;
    DataGenerator generator;
    auto colors = generator.parametric_generator(8, 0.25, 0.25, 0.25, 0.25).value();
    auto colors_cpy = colors;

    Graph graph;
    graph.gen_graph(colors);

    auto test = graph.perform_search(colors.size(), max_unsorted_length(colors));
    if(!test.has_value())
        return;
    InstructionList list2;
    for(auto &e : test.value()){
        list2.add_instruction(e);
    }

    simulate(colors_cpy, list2);
}


int main(/*int argc, char * argv[]*/)
{
    // For test purposes only
    no_args_run_uniform();
    no_args_run_substring();
    no_args_run_brute();

//    if(argc < 2){
//        std::cout << "Too few arguments" << std::endl;
//        return 0;
//    }
//    /*
//     * interactive mode
//     * ./aal_cmyk -i
//     */
//    if(cmdOptionExists(argv, argv + argc, "-i"))
//    {
//        std::cout << "interactive works" << std::endl;
//    }
//    /*
//    * instance mode
//    * ./aal_cmyk -s <problem size> <generator type> -f <result file>
//    */
//    else if(cmdOptionExists(argv, argv + argc, "-s"))
//    {
//        auto problemSize_ = argv[2];
//        auto generatorMode_ = argv[3];
//        char *filename_ = getCmdOption(argv, argv + argc, "-f");
//
//        int problemSize = atoi(problemSize_);
//        if(problemSize < 0){
//            std::cout << "Invalid problem size: " << problemSize << std::endl;
//            return 0;
//        }
//        int generatorMode = atoi(generatorMode_);
//        if(generatorMode < 0 || generatorMode > 3){
//            std::cout << "Invalid generator mode: " << generatorMode << std::endl;
//            return 0;
//        }
//        //generator execution
//        //algorithm execution
//
//        if(filename_){
//            // File result saving
//        }
//        else{
//            std::cout << "Output file was not specified" << std::endl;
//        }
//    }
//    /*
//    * testing mode
//    * ./aal_cmyk -t <problem size> <step> <iteration number> <generator type> -f <result file>
//    */
//    else if(cmdOptionExists(argv, argv + argc, "-t"))
//    {
//        auto problemSize_ = argv[2];
//        auto step_ = argv[3];
//        auto iterations_ = argv[4];
//        auto generatorMode_ = argv[5];
//        char *filename_ = getCmdOption(argv, argv + argc, "-f");
//
//        int problemSize = atoi(problemSize_);
//        int step = atoi(step_);
//        int iterations = atoi(iterations_);
//
//        if(problemSize < 0){
//            std::cout << "Invalid problem size: " << problemSize << std::endl;
//            return 0;
//        }
//        int generatorMode = atoi(generatorMode_);
//        if(generatorMode < 0 || generatorMode > 3){
//            std::cout << "Invalid generator mode: " << generatorMode << std::endl;
//            return 0;
//        }
//        if(step < 0){
//            std::cout << "Invalid step: " << step << std::endl;
//            return 0;
//        }
//        if(iterations < 0){
//            std::cout << "Invalid iterations: " << iterations << std::endl;
//            return 0;
//        }
//
//        if(filename_){
//            //File opening
//        }
//        else{
//            std::cout << "Output file was not specified" << std::endl;
//            return 0;
//        }
//
//        for(int i = 0; i < iterations; ++i){
//            //wywołanie generatora
//            //wywołanie algorytmu
//            //zapis do pliku
//        }
//    }
//    else if(cmdOptionExists(argv, argv + argc, "-h"))
//    {
//        std::cout << "Interactive mode:  " << "./aal_cmyk -i" << std::endl;
//        std::cout << "Instance mode:     " << "./aal_cmyk -s <problem size> <generator mode> -f <output file>" << std::endl;
//        std::cout << "Testing mode:      " << "./aal_cmyk -t <problem size> <step> <number of iterations> <generator mode> -f <output file>" << std::endl;
//    }
    return 0;
}