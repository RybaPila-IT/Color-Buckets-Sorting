//
// Created by swirta on 24.05.2021.
//

#include <iostream>
#include <unordered_map>
#include "alg_sort.hpp"
#include "constants.hpp"

Node::Node(std::deque<char> value_) : value(value_) {}

bool Graph::add_node_to_open_set(std::deque<Node> &openSet, Node &currentNode, Node &newNode) {
    bool equal = false;
    bool added = false;
    for(auto& e : currentNode.edges){
        if(graphNodes[e] == newNode)
            return false;
    }
    for(size_t i = 0; i< graphNodes.size(); ++i){
        if(graphNodes[i] == newNode){
            equal = true;
            currentNode.edges.push_back(i);
        }
    }
    if(!equal){
        auto index = graphNodes.empty() ? 0 : graphNodes.size() - 1;
        openSet.emplace_back(std::move(newNode));
        currentNode.edges.push_back(index);
        added = true;
    }
    return added;
}

std::vector<Node> Graph::node_permutation(Node &node) {
    std::vector<Node> nodeVector;
    for(size_t index = 0; index < 4; ++index){
        for(size_t i = index; i < node.value.size(); i = i + 4){
            if(i + 4 >= node.value.size())
                break;

            Node tmp = node;
            tmp.value.push_back(node.value[i]);
            tmp.value.push_back(node.value[i + 1]);
            tmp.value.push_back(node.value[i + 2]);
            tmp.value.push_back(node.value[i + 3]);
            for(size_t j = 0; j < 4; ++j)
                tmp.value.erase(tmp.value.begin() + i);

            nodeVector.push_back(tmp);
        }
    }
    return nodeVector;
}

void Graph::gen_graph(std::vector<char> &dataVector) {
    std::deque<char> data;
    for(auto &elem : dataVector){
        data.push_back(elem);
    }

    std::deque<Node> openSet;
    openSet.emplace_back(Node(data));
    while(!openSet.empty()){
        std::vector<Node> nodeVector = node_permutation(openSet.front());
        for(auto& potNode : nodeVector) {
            if(add_node_to_open_set(openSet, openSet.front(), potNode)){
                openSet.push_back(potNode);
                graphNodes.emplace_back(std::move(openSet.front()));
                openSet.pop_front();
            }
        }
    }
}

namespace {

    void robot_move(std::vector<char> &colors, uint idx) {

        for (uint i = 0; i < PATTERN_LEN; i++)
            colors.push_back(colors[idx + i]);

        colors.erase(colors.begin() + idx, colors.begin() + idx + PATTERN_LEN);
    }

    bool color_at_later_position(const std::vector<char> &colors, uint idx, char c) {

        for (uint i = idx + PATTERN_LEN; i < colors.size(); i += PATTERN_LEN)
            if (colors[i] == c)
                return true;

        return false;
    }

    void get_color(std::vector<char> &colors, uint l_end, char c, InstructionList &instructions) {

        while (colors[l_end] != c) {
            robot_move(colors, l_end);
            instructions.add_instruction(l_end);
        }
    }

    bool is_color(const std::unordered_map<char, uint>& occur, char c) {
        return occur.find(c)->second != 0;
    }

    void move_element_to_last_positions(std::vector<char> &colors, uint l_end, char c, InstructionList &instructions) {

        for (uint i = colors.size() - 1; i > l_end; i--)
            if (colors[i] == c) {

                if (i < colors.size() - PATTERN_LEN) {
                    robot_move(colors, i);
                    instructions.add_instruction(i);
                }

                return;
            }
    }

    void fit_element(std::vector<char> &colors, uint l_end, char c, InstructionList &instructions) {

        uint req_place = 0;

        for (uint i = colors.size() - 1; i >= colors.size() - PATTERN_LEN; i--)
            if (!((i - l_end) % PATTERN_LEN))
                req_place = i;

        while (colors[req_place] != c) {
            robot_move(colors, colors.size() - PATTERN_LEN - 1);
            instructions.add_instruction(colors.size() - PATTERN_LEN - 1);
        }
    }

    void move_color_to_required_position(std::vector<char> &colors, uint l_end, char c, InstructionList &instructions) {
        move_element_to_last_positions(colors, l_end, c, instructions);
        fit_element(colors, l_end, c, instructions);
    }

    std::unordered_map<char, uint> calculate_occur(const std::vector<char>& colors) {

        std::unordered_map<char, uint> occur;

        for (char i : PATTERN)
            occur.emplace(std::make_pair(i, 0));

        for (char c : colors)
            occur.find(c)->second++;

        return occur;
    }

}

InstructionList universal_sort(const std::vector<char>& colors) {

    InstructionList instructions;
    std::vector<char> colors_(colors.begin(), colors.end());
    std::unordered_map<char, uint> occur = calculate_occur(colors_);
    const uint max_sort = colors_.size() - PATTERN_LEN;

    for (uint l_end = 0, p_point = 0; l_end < max_sort; l_end++, p_point = (p_point + 1) % PATTERN_LEN) {

        // Color is not at it's place.
        if (colors_[l_end] != PATTERN[p_point]) {

            // Not found color at required further position.
            if (!color_at_later_position(colors_, l_end, PATTERN[p_point])) {

                if (!is_color(occur, PATTERN[p_point])) {
                    std::cout << "Warning: Lack of the " << PATTERN[p_point] << " character to end sorting; Terminating;" << std::endl;
                    break;
                }

                move_color_to_required_position(colors_, l_end, PATTERN[p_point], instructions);
            }

            get_color(colors_, l_end, PATTERN[p_point], instructions);
        }

        occur.find(colors_[l_end])->second--;
    }

    return instructions;
}


