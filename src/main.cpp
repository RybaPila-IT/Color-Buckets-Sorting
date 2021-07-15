#include <iostream>
#include <chrono>
#include <iomanip>
#include "data_generator.hpp"
#include "alg_sort.hpp"
#include "robot.hpp"

const char* LINE_ORNAMENT = "-----";
const char* INTERACTIVE_MODE_NAME = "INTERACTIVE MODE";
const char* AUTOMATIC_MODE_NAME = "AUTOMATIC MODE";

void print_diagnostics(const std::string& alg_name, const std::string &pre_sorted,
                       const std::string &sorted, size_t solution_size, const std::string& solution_string,
                       const std::chrono::duration<double, std::milli>& duration) {
    const uint print_width = 40;
    std::cout << std::left << std::setw(print_width) << "Problem instance's size:" << sorted.size() << std::endl;
    std::cout << std::left << std::setw(print_width) << "Algorithm's name:" << alg_name << std::endl;
    std::cout << std::left << std::setw(print_width) << "Problem instance:" << pre_sorted << std::endl;
    std::cout << std::left << std::setw(print_width) << "Result of algorithm's work:" << sorted << std::endl;
    std::cout << std::left << std::setw(print_width) << "Solution moves amount:" << solution_size << std::endl;
    std::cout << std::left << std::setw(print_width) << "Solution moves: " << solution_string << std::endl;
    std::cout << std::left << std::setw(print_width) << "Time of algorithm's work (ms):" << duration.count() << std::endl;
    std::cout << "\n\n";
}

void simulate_with_diagnostics(const std::string& algorithm_name, const std::vector<char>& colors,
                               const InstructionList& list,
                               const std::chrono::duration<double, std::milli>& duration) {
    auto to_sort = std::vector<char>(colors.begin(), colors.end());
    Robot::sort(to_sort, list);
    auto list_string = list.to_string();
    auto solution_size = list.size();
    auto sorted_string = std::string(to_sort.begin(), to_sort.end());
    auto pre_sorted_string = std::string(colors.begin(), colors.end());
    print_diagnostics(algorithm_name, pre_sorted_string, sorted_string, solution_size, list_string, duration);
}

// Checks whether all colors are one of the C,M,Y,K pattern.
bool input_string_colors_correctness(const std::string &data) {
    for(auto &color : data){
        if(std::find(PATTERN, PATTERN + 4, color) == std::end(PATTERN))
            return false;
    }
    return true;
}

void solve_problem_instance_with_algorithm(std::vector<char>& problem_instance, std::string& ask_prompt,
                                           std::string& algorithm_name,
                                           InstructionList (*algorithm)(const std::vector<char>&)) {
    std::string response;
    std::cout << ask_prompt;
    std::cin >> response;
    if (response == "y" || response == "Y") {
        auto begin = std::chrono::high_resolution_clock::now();
        auto orders_list = algorithm(problem_instance);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = end - begin;
        std::cout << LINE_ORNAMENT << algorithm_name << LINE_ORNAMENT << std::endl;
        simulate_with_diagnostics(algorithm_name, problem_instance, orders_list, duration);
    }
}


void use_sorting_algorithms_to_solve_problem_instance(std::vector<char>& problem_instance) {
    std::string universal_sort_prompt = "Use universal sort algorithm? (Y/N)";
    std::string substring_sort_prompt = "Use substring sort algorithm? (Y/N)";
    std::string bruteforce_sort_prompt = "Use brute force sort algorithm? (Y/N) WARNING: for problems bigger than 10 it may freeze computer.";
    std::string universal_sort_name = "universal sort";
    std::string substring_sort_name = "substring sort";
    std::string bruteforce_sort_name = "brute force sort";
    solve_problem_instance_with_algorithm(problem_instance, universal_sort_prompt, universal_sort_name, universal_sort);
    solve_problem_instance_with_algorithm(problem_instance, substring_sort_prompt, substring_sort_name, substrings_sort);
    solve_problem_instance_with_algorithm(problem_instance, bruteforce_sort_prompt, bruteforce_sort_name, brute_force_sort);
}

void handle_interactive_mode() {
    std::string response;
    std::cout << LINE_ORNAMENT << INTERACTIVE_MODE_NAME << LINE_ORNAMENT << std::endl;
    std::cout << "Please, enter the problem instance:" << std::endl;
    std::cin >> response;
    std::transform(response.begin(), response.end(), response.begin(), ::toupper);
    if (!input_string_colors_correctness(response)) {
        std::cout << "Provided response instance is incorrect (forbidden color occured). Terminating..." << std::endl;
        return;
    }
    std::vector<char> problem_instance(response.begin(), response.end());
    use_sorting_algorithms_to_solve_problem_instance(problem_instance);
}

DataGenerator* obtainParametricGenerator() noexcept(false) {
    double c_prob, m_prob, y_prob, k_prob;
    std::cout << "Provide C, M, Y probabilities. K probability will be deducted.";
    std::cin >> c_prob >> m_prob >> y_prob;
    if (m_prob <= 0.0 || c_prob < 0.0 || y_prob < 0.0) {
        std::cout << "Invalid generator parameters. Terminating...";
        throw std::runtime_error("Invalid generator parameters. At least one of them is negative.");
    }
    k_prob = 1.0 - c_prob - y_prob - m_prob;
    return new ParametricGenerator(c_prob, m_prob, y_prob, k_prob);
}

DataGenerator* obtainSubstringGenerator() noexcept(false) {
    double substr_prob;
    int substr_mean, substr_stddev;
    std::cout << "Provide substring probability, inner substring length mean and inner substring stddev:";
    std::cin >> substr_prob >> substr_mean >> substr_stddev;
    if (substr_prob < 0 || substr_mean <= 0 || substr_stddev <= 0) {
        throw std::runtime_error("Invalid generator parameters. At least one is negative.");
    }
    return new SubstringGenerator(substr_prob, substr_mean, substr_stddev);
}


void handle_automatic_mode() {
    int length;
    std::string response;
    DataGenerator* generator;
    std::cout << LINE_ORNAMENT << AUTOMATIC_MODE_NAME << LINE_ORNAMENT <<  std::endl;
    std::cout << "Please, enter the length of the problem instance:" << std::endl;
    std::cin >> length;
    if (length <= 0) {
        std::cout << "Invalid length provided. Terminating..." << std::endl;
        return;
    }
    std::cout << "Please choose the data generator: parametric (pr), substring (ss), permutation (pm): ";
    std::cin >> response;
    if (response == "pr") {
        try {
            generator = obtainParametricGenerator();
        } catch (std::runtime_error& e) {
            std::cout << e.what() << " Terminating...";
            return;
        }
    } else if (response == "ss") {
        try {
            generator = obtainSubstringGenerator();
        } catch (std::runtime_error& e) {
            std::cout << e.what() << " Terminating...";
            return;
        }
    } else if (response == "pm") {
        generator = new PermutationGenerator();
    } else {
        std::cout << "Provided invalid data generator format. Terminating..." << std::endl;
        return;
    }
    auto problem_instance = generator->generate(length);
    use_sorting_algorithms_to_solve_problem_instance(problem_instance);
}

int main() {
    std::string mode;
    std::cout << "Enter execution mode: (-i for interactive -a for automatic)\n";
    std::cin >> mode;
    if (mode == "-i")
        handle_interactive_mode();
    else if (mode == "-a")
        handle_automatic_mode();
    else
        std::cout << "Inserted mode is incorrect. Program terminates..." << std::endl;

    return 0;
}