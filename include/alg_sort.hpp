/**
 * @file alg_sort.hpp
 * @author Bartosz Świrta, Radosła Radziukiewicz
 * @brief Declarations of sort functions implemented in this program.
 * @version 1.0
 * @date 2021-06-06
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef AAL_CMYK_ALG_SORT_HPP
#define AAL_CMYK_ALG_SORT_HPP

#include <vector>
#include "robot.hpp"

InstructionList brute_force_sort(const std::vector<char>& colors);
InstructionList universal_sort(const std::vector<char>& colors);
InstructionList substrings_sort(const std::vector<char>& colors);

#endif //AAL_CMYK_ALG_SORT_HPP
