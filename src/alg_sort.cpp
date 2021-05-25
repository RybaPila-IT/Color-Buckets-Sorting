//
// Created by swirta on 24.05.2021.
//
#include "alg_sort.hpp"
#include "data_generator.hpp"

Node::Node(std::deque<char> value_) : value(value_) {}
Node::Node() {}

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
    gen_tree(root);
}

void Graph::gen_tree(Node &node) {
    std::vector<Node> nodeVector = node_permutation(node);
    for(auto& potNode : nodeVector) {
        add_node(node, potNode);
    }
    for(auto &child : node.childes)
        gen_tree(child);
}

bool Graph::perform_search(size_t length) {
    DataGenerator dataGenerator;
    std::deque<char> data;
    std::vector<char> dataVector = dataGenerator.substring_generator(length, 1.0);
    for(auto &elem : dataVector){
        data.push_back(elem);
    }
    Node node = Node(data);
    return find_node(root, node);
}
