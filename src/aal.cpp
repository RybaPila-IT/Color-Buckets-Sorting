#include "data_generator.hpp"
#include "alg_sort.hpp"
#include "robot.hpp"
#include "utils.hpp"
#include <iostream>
#include <chrono>
#include <cstring>
#include <optional>

char* getCmdOption(char ** begin, char ** end, const std::string & option)
{
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return nullptr;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}

void simulate(std::vector<char>& colors, InstructionList& list, bool interactive) {
    if(!interactive)
        Robot::sort(colors, list);
    else
        Robot::sort_interactive(colors, list);
}

void no_args_run_uniform() {

    std::cout << "Run uniform sort;" << std::endl;

    //std::vector<char> colors({'C', 'C', 'M','M', 'Y', 'Y', 'K', 'K'});
    //std::vector<char> colors({'C', 'Y', 'Y','M', 'C', 'M', 'Y', 'K'});
    ParametricGenerator generator;
    std::vector<char> colors;
    //colors = generator.parametric_generator(200, 0.25, 0.25, 0.25, 0.25).value();
    //colors = std::vector<char>({'M', 'C', 'Y', 'K', 'C', 'M', 'Y', 'K'});
    colors = generator.generate(200);
    auto original = colors;
    //colors = generator.substring_generator(80 , 1.0, 8, 5);
    auto begin = std::chrono::high_resolution_clock::now();
    auto list = universal_sort(colors);
    auto end = std::chrono::high_resolution_clock::now();

    simulate(colors, list, 0);

    auto time = get_time(begin, end);
    auto file = open_file("test.txt");
    if(file.has_value())
        print_diagnostics("test", original, colors, colors.size(), time, list.size(), file.value(), 1);

    print_diagnostics("uniform sort", original, colors, colors.size(), time, list.size(), std::cout, 0);
}

void no_args_run_substring() {

    std::cout << "Run substring sort;" << std::endl;

    SubstringGenerator generator(0.6, 8, 5);

    std::vector<char> colors;

    //colors = std::vector<char>({'C', 'M', 'Y', 'K', 'C', 'M', 'Y', 'K'});
    //colors = std::vector<char>({'M', 'M', 'Y', 'C', 'M', 'Y', 'K', 'C', 'M', 'M', 'C', 'M', 'Y'});
    //colors = std::vector<char>({'K', 'C', 'Y', 'K', 'C', 'M', 'Y', 'M'});
    //colors = std::vector<char>({'K', 'C', 'Y', 'C', 'C', 'M', 'Y', 'K', 'C', 'M', 'Y', 'K', 'Y', 'M', 'C', 'K', 'C', 'M','Y', 'K'});
    colors = generator.generate(200);
    auto original = colors;

    auto begin = std::chrono::high_resolution_clock::now();
    auto list = substrings_sort(colors);
    auto end = std::chrono::high_resolution_clock::now();
    auto time = get_time(begin, end);

    simulate(colors, list, 0);
    print_diagnostics("substring sort", original, colors, colors.size(), time, list.size(), std::cout, 0);
}

void no_args_run_brute() {

    std::cout << "Run brute-force sort" << std::endl;
    ParametricGenerator generator;
    auto colors = generator.generate(11);
    auto colors_cpy = colors;

    auto begin = std::chrono::high_resolution_clock::now();
    auto list = brute_force_sort(colors);
    auto end = std::chrono::high_resolution_clock::now();

    if(list.empty()){
        std::cout << "Nie udalo sie rozwiac problemu" << std::endl;
        return;
    }
    auto time = get_time(begin, end);
    simulate(colors_cpy, list, 0);
    print_diagnostics("brute force", colors, colors_cpy, colors.size(), time, list.size(), std::cout, 0);
}

int main(int argc, char * argv[])
{
    // For test purposes only
//    PermutationGenerator generator;
//    auto colors = generator.generate(8);
//    no_args_run_uniform();
//    no_args_run_substring();
//    no_args_run_brute();
    if(argc < 2){
        std::cout << "Too few arguments" << std::endl;
        return 0;
    }
    bool fileFlag = false;
    /*
     * interactive mode
     * ./aal_cmyk -i
     */
    if(cmdOptionExists(argv, argv + argc, "-i"))
    {
        std::cout << "---INTERACTIVE MODE---" << std::endl;
        std::cout << "Enter problem instance: ";
        std::string inputString;
        std::cin >> inputString;
        if(inputString.size() < 6){
            std::cout << "Problem cannot be solved" << std::endl;
            return 0;
        }
        std::transform(inputString.begin(), inputString.end(), inputString.begin(), ::toupper);
        if(check_string(inputString)){
            std::cout << "Incorrect input" << std::endl;
            return 0;
        }
        std::vector<char> colors;
        for(auto &elem : inputString){
            colors.push_back(elem);
        }
        auto universal = colors;
        auto universalList = universal_sort(universal);
        std::cout << "---universal sort---" << std::endl;
        simulate(universal, universalList, true);
        print_vector(universal, std::cout);

        auto substring = colors;
        auto substringList = substrings_sort(substring);
        std::cout << "---substring sort---" << std::endl;
        simulate(substring, substringList, true);
        print_vector(substring, std::cout);

        auto brute = colors;
        auto bruteList = brute_force_sort(brute);
        std::cout << "---brute force---" << std::endl;
        if(bruteList.empty()){
            std::cout << "Nie udalo sie rozwiac problemu" << std::endl;
            return 0;
        }
        simulate(brute, bruteList, true);
        print_vector(brute, std::cout);
    }
    /*
    * instance mode
    * ./aal_cmyk -s <problem size> <generator type> -us -ss -bs -f <result file>
    */
    else if(cmdOptionExists(argv, argv + argc, "-s"))
    {
        char *problemSize_;
        DataGenerator *generator;
        std::optional<std::fstream> filename;
        if(cmdOptionExists(argv, argv + argc, "-gp")){
            problemSize_ = getCmdOption(argv, argv + argc, "-gp");
            generator = new ParametricGenerator();
        }
        else if(cmdOptionExists(argv, argv + argc, "-gs")){
            problemSize_ = getCmdOption(argv, argv + argc, "-gs");
            generator = new SubstringGenerator(0.6, 8, 5);
        }
        else if(cmdOptionExists(argv, argv + argc, "-gns")){
            problemSize_ = getCmdOption(argv, argv + argc, "-gns");
            generator = new SubstringGenerator(0.0, 8, 5);
        }
        else{
            std::cout << "No generator was specified" << std::endl;
            return 0;
        }

        char *filename_ = getCmdOption(argv, argv + argc, "-f");
        if(filename_){
            filename = open_file(filename_);
            if(filename.has_value())
                fileFlag = true;
            else
                std::cout << "Could not open file" << filename_ << std::endl;
        }

        if(problemSize_){
            int problemSize = atoi(problemSize_);
            if(problemSize <= 0){
                std::cout << "Invalid problem size: " << problemSize << std::endl;
                delete generator;
                return 0;
            }
            auto colors = generator->generate(problemSize);

            if(cmdOptionExists(argv, argv + argc, "-us")){
                auto universal = colors;
                auto begin = std::chrono::high_resolution_clock::now();
                auto universalList = universal_sort(universal);
                auto end = std::chrono::high_resolution_clock::now();
                auto time = get_time(begin, end);

                simulate(universal, universalList, false);
                print_diagnostics("universal sort", colors, universal, colors.size(), time, universalList.size(), std::cout, 0);
                if(fileFlag)
                    print_diagnostics("universal sort", colors, universal, colors.size(), time, universalList.size(), filename.value(), 0);
            }
            if(cmdOptionExists(argv, argv + argc, "-ss")) {
                auto substring = colors;
                auto begin = std::chrono::high_resolution_clock::now();
                auto substringList = substrings_sort(substring);
                auto end = std::chrono::high_resolution_clock::now();
                auto time = get_time(begin, end);

                simulate(substring, substringList, false);
                print_diagnostics("substring sort", colors, substring, colors.size(), time, substringList.size(),
                                  std::cout, 0);
                if (fileFlag)
                    print_diagnostics("substring sort", colors, substring, colors.size(), time, substringList.size(),
                                      filename.value(), 0);
            }
            if(cmdOptionExists(argv, argv + argc, "-bs")) {
                auto brute = colors;
                auto begin = std::chrono::high_resolution_clock::now();
                auto bruteList = brute_force_sort(brute);
                auto end = std::chrono::high_resolution_clock::now();
                auto time = get_time(begin, end);
                if (bruteList.empty()) {
                    std::cout << "Nie udalo sie rozwiac problemu" << std::endl;
                    return 0;
                }

                simulate(brute, bruteList, false);
                print_diagnostics("brute force sort", colors, brute, colors.size(), time, bruteList.size(), std::cout,
                                  0);
                if (fileFlag)
                    print_diagnostics("brute force sort", colors, brute, colors.size(), time, bruteList.size(),
                                      filename.value(), 0);
            }
        }
        else
            std::cout << "Problem size was not specified" << std::endl;

        if(!fileFlag)
            std::cout << "Output file was not specified" << std::endl;
    }
    /*
    * testing mode
    * ./aal_cmyk -t -gp/-gs/-gns <problem size> <step> <number of iterations> -us -ss -bs -f <output file>
    */
    else if(cmdOptionExists(argv, argv + argc, "-t"))
    {
        char *problemSize_;
        DataGenerator *generator;
        std::optional<std::fstream> filename;
        if(cmdOptionExists(argv, argv + argc, "-gp")){
            problemSize_ = getCmdOption(argv, argv + argc, "-gp");
            generator = new ParametricGenerator();
        }
        else if(cmdOptionExists(argv, argv + argc, "-gs")){
            problemSize_ = getCmdOption(argv, argv + argc, "-gs");
            generator = new SubstringGenerator(0.6, 8, 5);
        }
        else if(cmdOptionExists(argv, argv + argc, "-gns")){
            problemSize_ = getCmdOption(argv, argv + argc, "-gns");
            generator = new SubstringGenerator(0.0, 8, 5);
        }
        else{
            std::cout << "No generator was specified" << std::endl;
            return 0;
        }

        char *filename_ = getCmdOption(argv, argv + argc, "-f");
        if(filename_){
            filename = open_file(filename_);
            if(filename.has_value())
                fileFlag = true;
            else
                std::cout << "Could not open file" << filename_ << std::endl;
        }
        auto step_ = argv[4];
        auto iterations_ = argv[5];

        if(problemSize_) {
            int problemSize = atoi(problemSize_);
            int step = atoi(step_);
            int iterations = atoi(iterations_);
            if (problemSize <= 0) {
                std::cout << "Invalid problem size: " << problemSize << std::endl;
                delete generator;
                return 0;
            }
            if(step <= 0){
                std::cout << "Invalid step: " << step << std::endl;
                return 0;
            }
            if(iterations <= 0){
                std::cout << "Invalid iterations: " << iterations << std::endl;
                return 0;
            }
            for(int i = 0; i < iterations; ++i){
                auto colors = generator->generate(problemSize + i*step);

                if(cmdOptionExists(argv, argv + argc, "-us")){
                    auto universal = colors;
                    auto begin = std::chrono::high_resolution_clock::now();
                    auto universalList = universal_sort(universal);
                    auto end = std::chrono::high_resolution_clock::now();
                    auto time = get_time(begin, end);

                    simulate(universal, universalList, false);
                    if(fileFlag)
                        print_diagnostics("universal sort", colors, universal, colors.size(), time, universalList.size(), filename.value(), 1);
                }
                if(cmdOptionExists(argv, argv + argc, "-ss")) {
                    auto substring = colors;
                    auto begin = std::chrono::high_resolution_clock::now();
                    auto substringList = substrings_sort(substring);
                    auto end = std::chrono::high_resolution_clock::now();
                    auto time = get_time(begin, end);

                    simulate(substring, substringList, false);
                    if (fileFlag)
                        print_diagnostics("substring sort", colors, substring, colors.size(), time, substringList.size(),filename.value(), 1);
                }
                if(cmdOptionExists(argv, argv + argc, "-bs")) {
                    auto brute = colors;
                    auto begin = std::chrono::high_resolution_clock::now();
                    auto bruteList = brute_force_sort(brute);
                    auto end = std::chrono::high_resolution_clock::now();
                    auto time = get_time(begin, end);
                    if (bruteList.empty()) {
                        std::cout << "Nie udalo sie rozwiac problemu" << std::endl;
                        return 0;
                    }

                    simulate(brute, bruteList, false);
                    if (fileFlag)
                        print_diagnostics("brute force sort", colors, brute, colors.size(), time, bruteList.size(),filename.value(), 1);
                }
            }
        }
        else
            std::cout << "Problem size was not specified" << std::endl;

        if(!fileFlag)
            std::cout << "Output file was not specified" << std::endl;
    }
    else if(cmdOptionExists(argv, argv + argc, "-h"))
    {
        std::cout << "Interactive mode:  " << "./aal_cmyk -i" << std::endl;
        std::cout << "Instance mode:     " << "./aal_cmyk -s <problem size> <generator type> -us -ss -bs -f <result file>" << std::endl;
        std::cout << "Testing mode:      " << "./aal_cmyk -t -gp/-gs/-gns <problem size> <step> <number of iterations> -us -ss -bs -f <output file>" << std::endl;
    }
    return 0;
}