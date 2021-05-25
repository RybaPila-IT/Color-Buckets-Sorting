//
// Created by swirta on 24.05.2021.
//

#ifndef AAL_CMYK_ALG_SORT_HPP
#define AAL_CMYK_ALG_SORT_HPP

#include <vector>
#include <deque>
#include "robot.h"

class Node{
public:
    bool operator==(const Node &right) const{
        return this->value == right.value;
    }

    Node(std::deque<char> value_);

    std::deque<char> value;
    std::vector<size_t> edges;
};

class Graph{
public:
    bool add_node_to_open_set(std::deque<Node> &openSet, Node &currentNode, Node &newNode);
    void gen_graph(std::vector<char> &dataVector);

    std::vector<Node> node_permutation(Node &node);

private:
    std::vector<Node> graphNodes;
};


InstructionList universal_sort(const std::vector<char>& colors);



#endif //AAL_CMYK_ALG_SORT_HPP
