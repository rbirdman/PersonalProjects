//
//  Coder.h
//  Huffman Encoding
//
//  Created by Ryan Bird on 11/26/13.
//  Copyright (c) 2013 Ryan Bird. All rights reserved.
//

#ifndef __Huffman_Encoding__Coder__
#define __Huffman_Encoding__Coder__

#include <iostream>
#include <fstream>

#include <map>
#include <string>
#include <sstream>

#include "HuffmanCode.h"

using namespace std;

class Coder {
    string input, output, encoding;
    string userInput;
    
    bool decodeFlag;
    
    //only needed to calculate mapping
    map<string, double> statistics;
    map<string, int> characterCount;
    int totalCount;
    
    //mapping, map char to bits, or string code?
    map<char, string> code;
    
    HuffmanCode huffmanCode;

    //encode
    void readEncodingFile(const string filename);
    void readEncodingData(istream&);
    
    void readInputFile(const string filename);
    void readInputData(istream&);
    
    void calcEncoding();
    void saveEncoding(string filename);
    
    //decode
    void readCodedFile(const string);
    void readCodedData(istream&);
    
public:
    
    Coder();
    Coder(string input, string output, string encoding);
    ~Coder();
    
    void encode();
    void decode();
    
    void printResult();
    void printResult(ostream&);
    void printStats(ostream&);
};

#endif /* defined(__Huffman_Encoding__Coder__) */
