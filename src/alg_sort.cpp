#include <iostream>
#include <unordered_map>
#include "alg_sort.hpp"
#include "constants.hpp"
#include "data_generator.hpp"

Node::Node(std::deque<char> value_) : index(0), value(value_), childes(std::vector<Node>()) {}

bool Node::equal(const Node &left, const Node &right, uint unordered){
    if(unordered > 4)
        return false;

    for(size_t i = 0; i < left.value.size() - unordered; ++i){
        if(left.value[i] != right.value[i])
            return false;
    }
    return true;
}

bool Graph::add_node(Node &currentNode, Node &newNode) {
    if(!find_node(root, newNode)){
        currentNode.childes.push_back(std::move(newNode));
        return true;
    }
    return false;
}

std::vector<Node> Graph::node_permutation(Node &node) {
    std::vector<Node> nodeVector;
    for(size_t index = 0; index < 4; ++index){
        for(size_t i = index; i < node.value.size(); i = i + 4){
            if(i + 4 >= node.value.size())
                break;

            Node tmp = node;
            tmp.index = i;
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

bool Graph::find_node(Node &parent, Node &node) {
    if(parent.value == node.value)
        return true;
    for(auto &node_ : parent.childes){
        if(find_node(node_, node))
            return true;
    }
    return false;
}

void Graph::gen_graph(std::vector<char> &dataVector) {
    std::deque<char> data;
    for(auto &elem : dataVector){
        data.push_back(elem);
    }
    root = Node(data);
    root.index = 0;
    gen_tree(root);
}

void Graph::gen_tree(Node &node) {
    std::vector<Node> nodeVector = node_permutation(node);

    for(auto& potNode : nodeVector)
        add_node(node, potNode);

    for(auto &child : node.childes)
        gen_tree(child);
}

bool Graph::find_with_trace(Node &parent, Node &node, uint unordered, std::deque<size_t> &instructions) {
    if(Node::equal(parent, node, unordered)){
        instructions.push_front(parent.index);
        return true;
    }

    for(auto &node_ : parent.childes){
        if(find_with_trace(node_, node, unordered, instructions)){
            instructions.push_front(parent.index);
            return true;
        }
    }
    return false;
}

std::optional<std::deque<size_t>> Graph::perform_search(size_t length, uint unordered) {
    DataGenerator dataGenerator;
    std::vector<char> dataVector = dataGenerator.substring_generator(length, 1.0, 10, 0);

    std::deque<char> dataDeque;
    std::cout << "TEST" << std::endl;
    for(auto &elem : dataVector){
        dataDeque.push_back(elem);
        std::cout << elem << ", ";
    }
    std::cout << std::endl;

    Node node = Node(dataDeque);
    std::deque<size_t> instructions;
    if(find_with_trace(root, node, unordered, instructions)){
        instructions.pop_front();
        return instructions;
    }
    return std::nullopt;
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

