//
// Created by radoslaw on 24.05.2021.
//
#include <stdexcept>
#include <utility>
#include "robot.h"

InstructionList::InstructionList(): current_instruction(0), instructions() {}


InstructionList::InstructionList(std::vector<uint> instructions): current_instruction(0), instructions(std::move(instructions)) {}

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

void Robot::sort(std::vector<char> &colors, InstructionList &instructions) {
    while (instructions.instruction_left()) {
        auto idx = instructions.get_next_instruction();

        for (uint i = 0; i < 4; i++)
            colors.push_back(colors[idx + i]);

        auto lower_bound = colors.begin() + idx;
        auto upper_bound = colors.begin() + idx + 4;

        colors.erase(lower_bound, upper_bound);
    }
}
