//
// Created by swirta on 24.05.2021.
//

#ifndef AAL_CMYK_ALG_SORT_HPP
#define AAL_CMYK_ALG_SORT_HPP

#include <vector>
#include <deque>
#include <memory>

class Node{
public:
    bool operator==(const Node &right) const{
        return this->value == right.value;
    }
    Node(std::deque<char> value_);
    Node();

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
    bool perform_search(size_t length);

    std::vector<Node> node_permutation(Node &node);
    Node root;
};

#endif //AAL_CMYK_ALG_SORT_HPP
