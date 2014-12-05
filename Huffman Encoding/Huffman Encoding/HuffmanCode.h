//
//  HuffmanCode.h
//  Huffman Encoding
//
//  Created by Ryan Bird on 11/30/13.
//  Copyright (c) 2013 Ryan Bird. All rights reserved.
//

#ifndef __Huffman_Encoding__HuffmanCode__
#define __Huffman_Encoding__HuffmanCode__

#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <map>

using namespace std;

//typedef struct {
//    string value;
//    double probability;
//} Probability;

class Probability {
public:
    string value;
    double probability;
    
    Probability();
    ~Probability();
    bool operator<(const Probability&) const;
    Probability operator+(const Probability&) const;
};

class Node {
    friend class HuffmanCode;
    Node* left; //1 bit
    Node* right; //0 bit
    
    Probability probability;
    
public:
    static bool compareNode(const Node*, const Node*);
};



class HuffmanCode {
    Node* root;
    map<string, string> codeMapping;
    
    void deleteNode(Node*);
    void createMapping();
    void addMapping(Node*, string);
    
public:
    HuffmanCode();
    ~HuffmanCode();
    
    void createTree(const vector<Probability>&);
    
    void print(ostream&);
    void printCode(ostream&);
    void printCode(ostream&, const map<string, double>& stats);
    string getCode(string input);
    
    void encode(istream&, ostream&);
    void decode(istream&, ostream&);
};

#endif /* defined(__Huffman_Encoding__HuffmanCode__) */
