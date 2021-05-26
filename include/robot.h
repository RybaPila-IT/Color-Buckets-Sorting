//
// Created by radoslaw on 24.05.2021.
//

#ifndef AAL_CMYK_ROBOT_H
#define AAL_CMYK_ROBOT_H

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
};

class Robot {

public:
    static void sort(std::vector<char>& colors, InstructionList& instructions);
};




#endif //AAL_CMYK_ROBOT_H
