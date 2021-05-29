#ifndef AAL_CMYK_CONSTANTS_HPP
#define AAL_CMYK_CONSTANTS_HPP

const char PATTERN[4] = {'C', 'M', 'Y', 'K'};
const int PATTERN_LEN = 4;
const char CSV_SEPARATOR = ';';

enum GEN_TYPE{
    RANDOM,
    SUBSTRING,
    NO_SUBSTRING,
    PERMUTATION
};

#endif //AAL_CMYK_CONSTANTS_HPP
