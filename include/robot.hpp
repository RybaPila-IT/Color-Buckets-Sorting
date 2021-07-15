#ifndef AAL_CMYK_ROBOT_HPP
#define AAL_CMYK_ROBOT_HPP

#include <vector>

class InstructionList {

private:
    uint current_instruction;
    std::vector<uint> instructions;

public:
    InstructionList();

    void add_instruction(uint order);
    uint get_next_instruction();
    [[nodiscard]] uint size() const;
    [[nodiscard]] bool instruction_left() const;
    [[nodiscard]] std::string to_string() const;
};

class Robot {

public:
    static void sort(std::vector<char>& colors, InstructionList instructions);
};

#endif //AAL_CMYK_ROBOT_HPP