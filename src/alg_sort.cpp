#include <iostream>
#include <unordered_map>
#include <map>
#include <queue>
#include "alg_sort.hpp"
#include "constants.hpp"
#include "utils.hpp"

bool equal(const std::deque<char> &left, const std::deque<char> &rigth, size_t unordered){
    for(size_t i = 0; i < rigth.size() - unordered; ++i){
        if(left[i] != rigth[i])
            return false;
    }
    return true;
}

std::deque<char> generate_terminal_node(size_t length){
    std::deque<char> terminalNode;
    terminalNode.emplace_back('C');
    while(terminalNode.size() != length){
        switch (terminalNode.back()) {
            case 'C':
                terminalNode.emplace_back('M');
                break;
            case 'M':
                terminalNode.emplace_back('Y');
                break;
            case 'Y':
                terminalNode.emplace_back('K');
                break;
            case 'K':
                terminalNode.emplace_back('C');
                break;
        }
    }
    return std::move(terminalNode);
}

std::vector<std::pair<std::deque<char>, InstructionList>> node_permutation(std::pair<std::deque<char>, InstructionList> &node) {
    std::vector<std::pair<std::deque<char>, InstructionList>> nodeVector;
    for(size_t index = 0; index < 4; ++index){
        for(size_t i = index; i < node.first.size(); i = i + 4){
            if(i + 4 >= node.first.size())
                break;

            auto newNode = node;
            newNode.second.add_instruction(i);
            newNode.first.push_back(node.first[i]);
            newNode.first.push_back(node.first[i + 1]);
            newNode.first.push_back(node.first[i + 2]);
            newNode.first.push_back(node.first[i + 3]);
            for(size_t j = 0; j < 4; ++j)
                newNode.first.erase(newNode.first.begin() + i);

            nodeVector.push_back(newNode);
        }
    }
    return std::move(nodeVector);
}

InstructionList brute_force_sort(const std::vector<char>& colors){
    /*Coping data to deque container for faster insert operations*/
    std::deque<char> colorsDeque;
    for(auto &color : colors)
        colorsDeque.push_back(color);

    auto terminalNode = generate_terminal_node(colors.size());
    auto unordered = max_unsorted_length(colors);
    std::map<std::deque<char>, InstructionList> search_tree;
    std::queue<std::pair<std::deque<char>, InstructionList>> openSet;

    /*initial vertex*/
    std::pair<std::deque<char>, InstructionList> root = std::pair(colorsDeque, InstructionList());
    search_tree.insert(root);
    openSet.push(root);

    while(!openSet.empty()){
        auto node = openSet.front();
        openSet.pop();
        if(equal(node.first, terminalNode, unordered))
            return node.second;

        auto permutations = node_permutation(node);
        for(auto &permutation : permutations){
            if(search_tree.insert(permutation).second)
                openSet.push(permutation);
        }
    }
    return std::move(InstructionList());
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

        for (uint i = colors.size() - 1 - PATTERN_LEN; i > l_end; i--)
            if (colors[i] == c) {

                robot_move(colors, i);
                instructions.add_instruction(i);

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

    std::unordered_map<char, uint> calculate_occur(const std::vector<char>& colors, uint sort_b) {

        std::unordered_map<char, uint> occur;

        for (char i : PATTERN) occur.emplace(std::make_pair(i, 0));
        for (uint i = sort_b; i < colors.size(); i++) occur.find(colors[i])->second++;

        return occur;
    }

    void _universal_sort(std::vector<char>& colors, uint sort_b, uint p_point_b, InstructionList& list) {

        std::unordered_map<char, uint> occur = calculate_occur(colors, sort_b);
        const uint max_sort = colors.size() - PATTERN_LEN;

        for (uint l_end = sort_b, p_point = p_point_b; l_end < max_sort; l_end++, p_point = (p_point + 1) % PATTERN_LEN) {

            // Color is not at it's place.
            if (colors[l_end] != PATTERN[p_point]) {

                // Not found color at required further position.
                if (!color_at_later_position(colors, l_end, PATTERN[p_point])) {

                    if (!is_color(occur, PATTERN[p_point])) {
                        std::cout << "Warning: Lack of the " << PATTERN[p_point] << " character to end sorting; Terminating;" << std::endl;
                        break;
                    }

                    move_color_to_required_position(colors, l_end, PATTERN[p_point], list);
                }

                get_color(colors, l_end, PATTERN[p_point], list);
            }

            occur.find(colors[l_end])->second--;
        }

    }



}

InstructionList universal_sort(const std::vector<char>& colors) {

    InstructionList instructions;
    std::vector<char> colors_(colors.begin(), colors.end());

    _universal_sort(colors_, 0, 0, instructions);

    return instructions;
}


namespace {

    void update_substring_found(int& substr_b, int& substr_l, int curr_substr_b, int curr_substr_l) {
        substr_l = std::max(substr_l, curr_substr_l);

        if (substr_l == curr_substr_l)
            substr_b = curr_substr_b;
    }

    void find_longest_substring(const std::vector<char>& colors, int l_end, int patt_ptr, int& substr_b, int& substr_l) {

        substr_b = l_end;
        substr_l = 0;

        bool set = false;
        int curr_substr_b = l_end;
        int curr_substr_l = 0;
        int curr_patt_ptr = 0;

        for (int i = l_end + PATTERN_LEN - 1; i < (int) colors.size() - PATTERN_LEN + 1; i++) {
            if (colors[i] == PATTERN[(patt_ptr + curr_patt_ptr) % 4]) {
                if (!set) {
                    curr_substr_b = i;
                    set = true;
                }
                curr_substr_l++;
                curr_patt_ptr++;
            } else {

                update_substring_found(substr_b, substr_l, curr_substr_b, curr_substr_l);
                curr_substr_b = i;
                curr_substr_l = 0;
                curr_patt_ptr = 0;
                set = false;
                // Decrement i if new subsequence begins.
                if (colors[i] == PATTERN[(patt_ptr + curr_patt_ptr) % 4])
                    i--;

            }
        }
        // Check whether substring ends with whole string itself.
        update_substring_found(substr_b, substr_l, curr_substr_b, curr_substr_l);
    }

    bool substring_at_position(int l_end, int subst_b) {
        return !((subst_b - l_end) % PATTERN_LEN);
    }

    void move_substring_to_position(std::vector<char>& colors, int l_end, int& substr_b, int substr_l, InstructionList& list) {

        int offset = 0;

        for (; (colors.size() - l_end - 1 - offset) % 4 != 0; offset++) {}

        int picking_pos = substr_b - PATTERN_LEN + 1 + offset;

        list.add_instruction(picking_pos);
        robot_move(colors, picking_pos);

        substr_l -= (1 + offset);
        substr_b = (int) colors.size() - 1 - offset;

        while (substr_l > 0) {
            if (substr_b -PATTERN_LEN > picking_pos) {
                list.add_instruction(picking_pos);
                robot_move(colors, picking_pos);

                substr_b -= PATTERN_LEN;
                substr_l -= PATTERN_LEN;
            } else
                break;
        }
    }

    void get_substring(std::vector<char>& colors, int l_end, int substr_b, InstructionList& list) {

        for (int i = substr_b; i != l_end; i -= PATTERN_LEN) {
            list.add_instruction(l_end);
            robot_move(colors, l_end);
        }

    }

}

InstructionList substrings_sort(const std::vector<char>& colors) {

    InstructionList list;
    std::vector<char> colors_(colors.begin(), colors.end());

    int l_end = 0, patt_ptr = 0;
    int substr_b, substr_l;

    const int unsorted_threshold = 2 * PATTERN_LEN - 2;

    do {

        // Color is not at it's place.
        if (colors_[l_end] != PATTERN[patt_ptr]) {

            find_longest_substring(colors_, l_end, patt_ptr, substr_b, substr_l);

            if (!substr_l) {
                // If why did not find required substring it means that there is only few (in unchecked boarders) or none
                // required color bins left. We break the loop in order to finish sorting with the universal sort.
                break;
            } else {

                if (!substring_at_position(l_end, substr_b))
                    move_substring_to_position(colors_, l_end, substr_b, substr_l, list);

                get_substring(colors_, l_end, substr_b, list);

                l_end += substr_l;
                patt_ptr = (patt_ptr + substr_l) % 4;

            }

        } else {
            l_end++;
            patt_ptr = (patt_ptr + 1) % PATTERN_LEN;
        }

    } while (l_end < (int) colors_.size() - unsorted_threshold);


    _universal_sort(colors_, l_end, patt_ptr, list);

    return list;
}

