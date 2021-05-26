#ifndef AAL_CMYK_ALG_SORT_HPP
#define AAL_CMYK_ALG_SORT_HPP

#include <vector>
#include <deque>
#include <optional>
#include "robot.h"

class Node{
public:
    explicit Node(std::deque<char> value_);
    Node() = default;

    bool operator==(const Node &right) const{
        return this->value == right.value;
    }
    static bool equal(const Node &left, const Node &right, uint unordered);

    size_t index;
    std::deque<char> value;
    std::vector<Node> childes;
};

class Graph{
public:
    bool add_node(Node &currentNode, Node &newNode);
    void gen_graph(std::vector<char> &dataVector);
    void gen_tree(Node &node);
    bool find_node(Node &parent, Node &node);
    std::vector<Node> node_permutation(Node &node);

    bool find_with_trace(Node &parent, Node &node, uint unordered, std::deque<size_t> &instructions);
    std::optional<std::deque<size_t>> perform_search(size_t length, uint unordered);

    Node root;
};

InstructionList universal_sort(const std::vector<char>& colors);
InstructionList substrings_sort(const std::vector<char>& colors);

#endif //AAL_CMYK_ALG_SORT_HPP
