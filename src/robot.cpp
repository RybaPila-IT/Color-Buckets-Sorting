#include <stdexcept>
#include <iostream>
#include "robot.hpp"
#include "constants.hpp"
#include "utils.hpp"

InstructionList::InstructionList(): current_instruction(0), instructions() {}

bool InstructionList::empty() {
    return instructions.empty();
}

void InstructionList::add_instruction(uint order) {
    instructions.push_back(order);
}

uint InstructionList::get_next_instruction() {
    if (!instruction_left())
        throw std::runtime_error("Error: no instruction left");

    return instructions[current_instruction++];
}

bool InstructionList::instruction_left() const {
    return current_instruction < instructions.size();
}

uint InstructionList::size() const {
    return instructions.size();
}

void Robot::sort(std::vector<char> &colors, InstructionList &instructions) {
    while (instructions.instruction_left()) {
        auto idx = instructions.get_next_instruction();

        for (uint i = 0; i < PATTERN_LEN; i++)
            colors.push_back(colors[idx + i]);

        colors.erase(colors.begin() + idx, colors.begin() + idx + PATTERN_LEN);
    }
}
void Robot::sort_interactive(std::vector<char>& colors, InstructionList& instructions){
    while (instructions.instruction_left()) {
        auto idx = instructions.get_next_instruction();

        print_vector(colors, std::cout);

        for (uint i = 0; i < PATTERN_LEN; i++)
            colors.push_back(colors[idx + i]);

        colors.erase(colors.begin() + idx, colors.begin() + idx + PATTERN_LEN);
    }
}
