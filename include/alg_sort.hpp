#ifndef AAL_CMYK_ALG_SORT_HPP
#define AAL_CMYK_ALG_SORT_HPP

#include <vector>
#include "robot.hpp"

InstructionList brute_force_sort(const std::vector<char>& colors);
InstructionList universal_sort(const std::vector<char>& colors);
InstructionList substrings_sort(const std::vector<char>& colors);

#endif //AAL_CMYK_ALG_SORT_HPP
