//
//  HuffmanCode.cpp
//  Huffman Encoding
//
//  Created by Ryan Bird on 11/30/13.
//  Copyright (c) 2013 Ryan Bird. All rights reserved.
//

#include "HuffmanCode.h"

Probability::Probability() : probability(0) {
}
Probability::~Probability() {
}

Probability Probability::operator+(const Probability& other) const {
    Probability toReturn;
    
    toReturn.probability = probability + other.probability;
    toReturn.value = value + "&" + other.value;
    
    return toReturn;
}

bool Probability::operator<(const Probability & other) const {
    return this->probability <= other.probability;
}

bool Node::compareNode(const Node* first, const Node* second) {
    return second->probability < first->probability; //min priority queue
}

HuffmanCode::HuffmanCode() : root(NULL) {
    
}

HuffmanCode::~HuffmanCode() {
    deleteNode(root);
}

void HuffmanCode::deleteNode(Node* node) {
    if (node != NULL) {
        deleteNode(node->left);
        deleteNode(node->right);
        delete node;
    }
}

class NodeCompare {
public:
    bool operator() (const Node* first, const Node* second) const {
        return Node::compareNode(first, second);
    }
};

void HuffmanCode::createTree(const vector<Probability>& values) {
//    priority_queue<Node*> nodes(&Node::compareNode);
//    priority_queue<Node*> nodes(Node::compareNode, vector<Node*>);
    priority_queue<Node*, vector<Node*>, NodeCompare> nodes;
    
    for (int i = 0; i < values.size(); i++) {
        Node* node = new Node();
        node->probability = values[i];
        nodes.push(node);
    }
    
    while (nodes.size() > 1) {
        Node* firstChild = nodes.top();
        nodes.pop();
        Node* secondChild = nodes.top();
        nodes.pop();
        
        Node* parent = new Node();
        parent->right = firstChild;
        parent->left = secondChild;
        
        parent->probability = firstChild->probability + secondChild->probability;
        
        nodes.push(parent);
    }
    
    //root node
    root = nodes.top();
    createMapping();
}

void HuffmanCode::createMapping() {
    addMapping(root, "");
}

void HuffmanCode::addMapping(Node* node, string code) {
    //assumption: codes only exist in leaf nodes
    if (node == NULL) {
        return;
    }
    else if (node->left == NULL && node->right == NULL) {
        //code
        codeMapping[node->probability.value] = code;
    }
    else {
        addMapping(node->left, code + "1");
        addMapping(node->right, code + "0");
    }
}

void HuffmanCode::print(ostream& output) {
    queue<Node*> curr;
    queue<Node*> nextLevel;
    
    curr.push(root);
    while (curr.size()) {
        Node* currNode = curr.front();
        curr.pop();
        
        cout << " " << currNode->probability.value << "(" << currNode->probability.probability << ")";
        
        if (currNode->left != NULL)
            nextLevel.push(currNode->left);
        if (currNode->right != NULL) {
            nextLevel.push(currNode->right);
        }
        
        if (curr.size() == 0) {
            curr = nextLevel;
            cout << endl;
            nextLevel = queue<Node*>();
        }
    }
}

string HuffmanCode::getCode(string input) {
    return codeMapping[input];
}

void HuffmanCode::printCode(ostream& output) {
    map<string, string>::iterator iter = codeMapping.begin();
    
    for (; iter != codeMapping.end(); iter++) {
        output << iter->first << ": " << iter->second << endl;
    }
}

void HuffmanCode::printCode(ostream& output, const map<string, double>& stats) {
    map<string, string>::iterator iter = codeMapping.begin();
    
    for (; iter != codeMapping.end(); iter++) {
        output << iter->first << ": " << iter->second;
        output << " " << stats.at(iter->first) << endl;
    }
}

void HuffmanCode::encode(istream& input, ostream& output) {
    //convert char to bits
    string runningBits;
    for (char c = input.get(); input.good(); c = input.get()) {
        string value;
        value += c;
        runningBits += getCode(value);
        if (runningBits.size() >= 8) {
            //print
            
            //take out 8 bits
        }
    }
    
    if (runningBits.size()) {
        //print out remaining bits
    }
}

void HuffmanCode::decode(istream& input, ostream& output) {
    //convert bits to char
    for (char c = input.get(); input.good(); c = input.get()) {
        //iterate through tree, remember place in tree
    }
}
