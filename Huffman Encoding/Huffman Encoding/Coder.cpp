//
//  Coder.cpp
//  Huffman Encoding
//
//  Created by Ryan Bird on 11/26/13.
//  Copyright (c) 2013 Ryan Bird. All rights reserved.
//

#include "Coder.h"

Coder::Coder() {
    
}

Coder::Coder(string input, string output, string encoding) : input(input), output(output), encoding(encoding), decodeFlag(false) {
    if (output.size() == 0)
        output = "encode.txt";
}

Coder::~Coder() {
    
}

void Coder::encode() {
    decodeFlag = false;
    
    if (input.size() == 0)
        readInputData(cin);
    else
        readInputFile(input);
    
    if (encoding.size())
        readEncodingFile(encoding);
    else {
        calcEncoding();
        saveEncoding("encode-"+input);
    }
//    cout << "Stats:" << endl;
//    printStats(cout);
//    cout << endl;
    
    
//    huffmanCode.print(cout);
    
//    cout << endl;
//    huffmanCode.printCode(cout);
    huffmanCode.printCode(cout, statistics);
}

void Coder::decode() {
    decodeFlag = true;
    if (encoding.size())
        readEncodingFile(encoding);
    else
        readEncodingData(cin);
    
    if (input.size() == 0)
        readCodedData(cin);
    else
        readCodedFile(input);
}

void Coder::readEncodingFile(const string filename) {
    ifstream file(filename);
    readEncodingData(file);
}

void Coder::readEncodingData(istream& input) {
    string line;
    vector<Probability> probabilities;
    
    while (getline(input, line)) {
        string code;
        stringstream str(line);
        
        Probability temp;
        str >> temp.value >> code >> temp.probability;
        
        temp.value = temp.value.substr(0,temp.value.size() - 1);
        probabilities.push_back(temp);
        statistics[temp.value] = temp.probability;
    }
    
    huffmanCode.createTree(probabilities);
}

void Coder::readInputFile(const string filename) {
    ifstream file(filename);
    readInputData(file);
}

void Coder::readInputData(istream& data) {
    for (char c = data.get(); data.good(); c = data.get()) {
        string val;
        val += c;
        characterCount[val]++;
//        characterCount[c]++;
        totalCount++;
        userInput += c;
    }
}

void Coder::calcEncoding() {
    map<string, int>::iterator iter = characterCount.begin();
    vector<Probability> probabilities;
    
    for (; iter != characterCount.end(); iter++) {
        statistics[iter->first] = ((double) iter->second) / totalCount;
        Probability temp;
        temp.value = iter->first;
        temp.probability = ((double) iter->second) / totalCount;
        probabilities.push_back(temp);
    }
    
    huffmanCode.createTree(probabilities);
}

void Coder::saveEncoding(string filename) {
    ofstream file(filename);
    huffmanCode.printCode(file, statistics);
}

//decode
void Coder::readCodedFile(const string filename) {
    ifstream file(filename);
    readCodedData(file);
}

void Coder::readCodedData(istream& input) {
    for (char c = input.get(); input.good(); c = input.get()) {
        bitset<sizeof(char)*8> bits(c);
        
        for (int i = bits.size() - 1; i >= 0; i--) {
            //            cout << ((bits[i])?1:0);
            cout << bits[i];
        }
        cout << endl;
    }
}

void Coder::printResult() {
    if (output.size() == 0) {
        printResult(cout);
    }
    else {
        ofstream file(output);
        printResult(file);
    }
}

void Coder::printResult(ostream& print) {
//    huffmanCode.encode(<#istream &#>, <#ostream &#>);
//    huffmanCode.decode(<#istream &#>, <#ostream &#>);
    stringstream str(userInput);
    if (decodeFlag) {
        huffmanCode.decode(str, print);
    }
    else {
        huffmanCode.encode(str, print);
    }
}

void Coder::printStats(ostream& print) {
    map<string, double>::iterator statsIter = statistics.begin();
    for (; statsIter != statistics.end(); statsIter++)
        cout << statsIter->first << ": " << statsIter->second << endl;
}
