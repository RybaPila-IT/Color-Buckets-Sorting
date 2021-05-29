//
// Created by radoslaw on 24.05.2021.
//

#ifndef AAL_CMYK_ROBOT_HPP
#define AAL_CMYK_ROBOT_HPP

#include <vector>


class InstructionList {

private:
    uint current_instruction;
    std::vector<uint> instructions;

public:
    InstructionList();

    bool empty();
    void add_instruction(uint order);
    uint get_next_instruction();
    [[nodiscard]] uint size() const;
    [[nodiscard]] bool instruction_left() const;
};

class Robot {

public:
    static void sort(std::vector<char>& colors, InstructionList& instructions);
    static void sort_interactive(std::vector<char>& colors, InstructionList& instructions);
};




#endif //AAL_CMYK_ROBOT_HPP
