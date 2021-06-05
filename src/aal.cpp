#include "data_generator.hpp"
#include "alg_sort.hpp"
#include "robot.hpp"
#include "utils.hpp"
#include <iostream>
#include <chrono>
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

int main(int argc, char * argv[])
{
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
    * ./aal_cmyk -s -gp/-gs/-gns <problem size> -us -ss -bs -f <result file>
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
                print_diagnostics("universal sort", colors, universal, colors.size(), time, universalList.size(), std::cout);
                if(fileFlag)
                    print_diagnostics("universal sort", colors, universal, colors.size(), time, universalList.size(), filename.value());
            }
            if(cmdOptionExists(argv, argv + argc, "-ss")) {
                auto substring = colors;
                auto begin = std::chrono::high_resolution_clock::now();
                auto substringList = substrings_sort(substring);
                auto end = std::chrono::high_resolution_clock::now();
                auto time = get_time(begin, end);

                simulate(substring, substringList, false);
                print_diagnostics("substring sort", colors, substring, colors.size(), time, substringList.size(),std::cout);
                if (fileFlag)
                    print_diagnostics("substring sort", colors, substring, colors.size(), time, substringList.size(),filename.value());
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
                print_diagnostics("brute force sort", colors, brute, colors.size(), time, bruteList.size(), std::cout);
                if (fileFlag)
                    print_diagnostics("brute force sort", colors, brute, colors.size(), time, bruteList.size(),filename.value());
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
        char *initProblemSize_;
        DataGenerator *generator;
        std::optional<std::fstream> filename;
        if(cmdOptionExists(argv, argv + argc, "-gp")){
            initProblemSize_ = getCmdOption(argv, argv + argc, "-gp");
            generator = new ParametricGenerator();
        }
        else if(cmdOptionExists(argv, argv + argc, "-gs")){
            initProblemSize_ = getCmdOption(argv, argv + argc, "-gs");
            generator = new SubstringGenerator(0.6, 8, 5);
        }
        else if(cmdOptionExists(argv, argv + argc, "-gns")){
            initProblemSize_ = getCmdOption(argv, argv + argc, "-gns");
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

        if(initProblemSize_) {
            int initProblemSize = atoi(initProblemSize_);
            int step = atoi(step_);
            int iterations = atoi(iterations_);
            if (initProblemSize <= 0) {
                std::cout << "Invalid problem size: " << initProblemSize << std::endl;
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
                auto problemSize = initProblemSize + i*step;
                auto universalTime = std::chrono::duration<double, std::milli>(),
                        substringTime = std::chrono::duration<double, std::milli>(),
                        bruteTime = std::chrono::duration<double, std::milli>();

                for(int j = 0; j < RERUN; ++j) {
                    auto colors = generator->generate(problemSize);

                    if (cmdOptionExists(argv, argv + argc, "-us")) {
                        auto universal = colors;
                        auto begin = std::chrono::high_resolution_clock::now();
                        auto universalList = universal_sort(universal);
                        auto end = std::chrono::high_resolution_clock::now();
                        auto time = get_time(begin, end);

                        universalTime = std::max(universalTime, time);

//                        simulate(universal, universalList, false);
                        if (fileFlag && j == RERUN - 1){
                            print_diagnostics(colors.size(), universalTime, universalList.size(), filename.value());
                            universalTime = std::chrono::duration<double, std::milli>::zero();
                        }
                    }
                    if (cmdOptionExists(argv, argv + argc, "-ss")) {
                        auto substring = colors;
                        auto begin = std::chrono::high_resolution_clock::now();
                        auto substringList = substrings_sort(substring);
                        auto end = std::chrono::high_resolution_clock::now();
                        auto time = get_time(begin, end);

                        substringTime = std::max(substringTime, time);

//                        simulate(substring, substringList, false);
                        if (fileFlag && j == RERUN - 1) {
                            print_diagnostics(colors.size(), substringTime, substringList.size(), filename.value());
                            substringTime = std::chrono::duration<double, std::milli>::zero();
                        }
                    }
                    if (cmdOptionExists(argv, argv + argc, "-bs") && problemSize < 20) {
                        auto brute = colors;
                        auto begin = std::chrono::high_resolution_clock::now();
                        auto bruteList = brute_force_sort(brute);
                        auto end = std::chrono::high_resolution_clock::now();
                        auto time = get_time(begin, end);

                        
                        if (bruteList.empty())
                            std::cout << "Nie udalo sie rozwiac problemu" << std::endl;
                        else
                            bruteTime = std::max(bruteTime, time);

//                        simulate(brute, bruteList, false);
                        if (fileFlag && j == RERUN - 1){
                            print_diagnostics(colors.size(), bruteTime, bruteList.size(), filename.value());
                            bruteTime = std::chrono::duration<double, std::milli>::zero();
                        }
                    }
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
        std::cout << "Instance mode:     " << "./aal_cmyk -s -gp/-gs/-gns <problem size> -us -ss -bs -f <result file>" << std::endl;
        std::cout << "Testing mode:      " << "./aal_cmyk -t -gp/-gs/-gns <problem size> <step> <number of iterations> -us -ss -bs -f <output file>" << std::endl;
    }
    return 0;
}