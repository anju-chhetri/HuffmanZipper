#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <bitset>
#include <unordered_map>
#include "MinHeapNode.h"
#include "MinHeap.cpp"
#define INTERNAL_NODE_CHARACTER char(128)
#define PSEUDO_EOF char(129)
#define CHARACTER_SEPARATOR char(130)
#define VALUE_SEPARATOR char(131)
#define HEADER_SEPARATOR char(132)
using namespace std;

void createcodeMap(unordered_map<char, int> frequencyMap)
{
     for(const auto &item: frequencyMap)
    {
        cout<<item.first<<"  "<<item.second<<"\n";
    }

    int size = frequencyMap.size();
    char arr[size+1];
    int freq[size+1];
    int i=0;
    for(const auto &item: frequencyMap)
    {
        arr[i] = item.first;
        freq[i] = item.second;
        i++;
    }
    arr[i] = PSEUDO_EOF;//later we want to make a node for PSEUDO_EOF in the huffman tree
    //we are going to keep PSEUDO_EOF "binary_value" at the end of the compressed file to mark end of "binary_values" for input.txt or to
    // separate it from padding bits(so that we mistakely do not decode padding bits)
    freq[i] = 1;


    // Construct Huffman Tree or encoding tree
    struct MinHeapNode* root
        = buildHuffmanTree(arr, freq, size+1);

    string tempString;//empty string
    encodeCharacters(root, tempString);//this is a recursive function that calls itself until all characters get their binary values

    for (const auto &item: codeMap) {//prints out the codeMap
    cout << "{" << item.first << ": " << item.second << "}\n";
    }
}


void writeHeader(ofstream &outputStream) {//used to store the codeMap in compressed.txt
    for (const auto &item : codeMap)
        outputStream << item.first << CHARACTER_SEPARATOR << item.second << VALUE_SEPARATOR;//CHARACTER_SEPARATOR comes after each characters
        //VALUE_SEPARATOR comes after "binary value" for each character
    outputStream << HEADER_SEPARATOR;//HEADER_SEPARATOR comes at the end of codeMap
}

void compressTofile(string Inputfile ,string Outputfile) {

    char character;
    string fileString;
    ifstream inputStream;
    ofstream outputStream;
    outputStream.open(Outputfile, ios::out);
    inputStream.open(Inputfile, ios::in);
    writeHeader(outputStream);
    while (inputStream.get(character))//read a character from inputFile
        fileString += codeMap[character];//fileString is a string where binary value of character just read is added
    inputStream.close();

    //add "binary value" of PSEUDO_EOF to mark end of "binary_values" for input.txt
    fileString += codeMap[PSEUDO_EOF];



    //We can write or read to or from file only if the bits we want to read or write are multiple of 8 bits
    //While writing we make a pack of 8 bits, make a character out of it and write the character to the file
    //So we check how many bits are we deficit to make fileString(bits to be stored) a multiple of 8 bits

    unsigned long remainder = (fileString.size() - 1) % 8;//remainder is the number of deficit bits
    for (int i = 0; i < 8 - remainder; ++i)
        fileString += '0';//add '0' as the padding bits to the fileString


    stringstream strStream(fileString);

    while (strStream.good()) {//while strStream has proper values
        bitset<8> bits;//Bitset is a container in C++ Standard Template Library for dealing with data at the bit level
        strStream >> bits;//here bits hold 8 bits extracted from strStream
        char c = char(bits.to_ulong());//pack the 8 bits as a character
        outputStream << c;//write the character in the outputFile
    }


    outputStream.flush();
    outputStream.close();
}

void readHeader(ifstream &inputStream) {
    codeMap.clear();//clear codeMap that were built and used for compression of previous text file
    char character;
    inputStream.get(character);
    char key = character;//key for the codeMap
    while (character != HEADER_SEPARATOR) {//HEADER_SEPARATOR marks the end of codeMap
        if (character == CHARACTER_SEPARATOR) {
            inputStream.get(character);
            while (character != VALUE_SEPARATOR) {//VALUE_SEPARATOR marks the end of "binary value" of the character
                codeMap[key] += character;
                inputStream.get(character);
            }
        } else//if it is not a CHARACTER_SEPARATOR i.e if it is a character
            key = character;
        inputStream.get(character);
    }
}

struct MinHeapNode* buildDecodingTree() {

    struct MinHeapNode* rootNode = newNode(INTERNAL_NODE_CHARACTER);
    struct MinHeapNode* previousNode;

    for (const auto &item : codeMap) {
        previousNode = rootNode;
        struct MinHeapNode* new_node = newNode(item.first);
        string characterCode = item.second;
        for (int i = 0; i < characterCode.size(); ++i) {
            if (characterCode[i] == '0') {
                if (i == characterCode.size() - 1)
                    //previousNode->setLeft(newNode);
                    previousNode->left = new_node;
                else {
                    if (!previousNode->left) {
                        previousNode->left = newNode(INTERNAL_NODE_CHARACTER);
                        previousNode = previousNode->left;
                    } else previousNode = previousNode->left;
                }
            } else {
                if (i == characterCode.size() - 1)
                    previousNode->right = new_node;
                else {
                    if (!previousNode->right) {
                        previousNode->right = newNode(INTERNAL_NODE_CHARACTER);
                        previousNode = previousNode->right;
                    } else previousNode = previousNode->right;
                }
            }
        }

    }
    return rootNode;
}

void decompressToFile(string codeString, struct MinHeapNode* rootNode, string decompressedFileName) {
    ofstream outputStream;
    outputStream.open(decompressedFileName, ios::out);
    struct MinHeapNode* traversingPointer = rootNode;
    for (int i = 0; i < codeString.size() + 1; ++i) {
        if (codeString[i] == '0')
            traversingPointer = traversingPointer->left;
        else
            traversingPointer = traversingPointer->right;

        if (traversingPointer->data != INTERNAL_NODE_CHARACTER) //i.e it is leaf nodes with characters like A, B, C
        {
            if (traversingPointer->data == PSEUDO_EOF)
                break;
            outputStream << traversingPointer->data;
            traversingPointer = rootNode;
        }
    }
    outputStream.flush();
    outputStream.close();

}

void Decipher(string compressedFileName, string decompressedFileName) {
    char character;
    string codeString;
    ifstream inputStream;
    inputStream.open(compressedFileName, ios::in);
    readHeader(inputStream);
    while (inputStream.get(character)) {
        bitset<8> bits(character);
        codeString += bits.to_string();

    }
    struct MinHeapNode*rootNode = buildDecodingTree();
    decompressToFile(codeString, rootNode, decompressedFileName);


}
