//
// Created by swirta on 24.05.2021.
//

#include "alg_sort.hpp"

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


