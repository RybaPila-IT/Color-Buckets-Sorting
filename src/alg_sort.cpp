//
// Created by swirta on 24.05.2021.
//
#include "alg_sort.hpp"
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
    std::vector<char> dataVector = dataGenerator.substring_generator(length, 1.0);

    std::deque<char> dataDeque;
    for(auto &elem : dataVector){
        dataDeque.push_back(elem);
    }

    Node node = Node(dataDeque);
    std::deque<size_t> instructions;
    if(find_with_trace(root, node, unordered, instructions)){
        instructions.pop_front();
        return instructions;
    }
    return std::nullopt;
}
