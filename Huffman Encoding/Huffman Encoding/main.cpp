//
//  main.cpp
//  Huffman Encoding
//
//  Created by Ryan Bird on 11/19/13.
//  Copyright (c) 2013 Ryan Bird. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <unistd.h>


#include <bitset>
#include <string>
#include <map>

#include "Coder.h"


using namespace std;

const char USAGE[] = "encode [-i inputfile] [-o output] [-e encoding]";

map<char, double> statistics;
map<char, int> characterCount;
int totalCount(0);

void encode(string inputFile, string outputFile, string encodingFile);
void decode(string inputFile, string outputFile, string encodingFile);

void readEncodingFile(const string filename);
void readEncodingData(istream&);

void readInputFile(const string filename);
void readInputData(istream&);

void calcEncoding();
void saveEncoding(string filename);
void printEncodedData(ostream&);

//decode
void printDecodedData(ostream&);
void readCodedFile(const string);
void readCodedData(istream&);

int main(int argc, const char * argv[]) {
    int option;
    string input, output, encoding;
    bool decodeFlag(false);
    
    //one mode for encryption and one mode for decryption?
    while ((option = getopt(argc,(char**)argv,"i:o:e:d")) != -1) {
        switch (option) {
            //option, read from std::in otherwise? file contents can be piped
            //std::in reads from pipe
            //ie. echo "hello" | test.exe
            case 'i': //input file
                input = optarg;
                break;
                //output to std::out otherwise? can always pip to a file
            case 'o': //output file
                output = optarg;
                break;
            case 'e': //encoding file
                encoding = optarg;
                break;
            case 'd':
                decodeFlag = true;
                break;
            default:
                cout << USAGE << endl;
                exit(EXIT_FAILURE);
        }
    }
    
    Coder code(input, output, encoding);
    
    if (decodeFlag)
        code.decode();
    else
        code.encode();
    
    code.printResult();
    
    return 0;
}
