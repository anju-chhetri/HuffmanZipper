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
    arr[i] = PSEUDO_EOF;//we are going to keep PSEUDO_EOF at the end of the compressed file to separate padding bits(so that we mistakely do not decode padding bits
    freq[i] = 1;
    //int size = sizeof(arr) / sizeof(arr[0]);

    // Construct Huffman Tree
    struct MinHeapNode* root
        = buildHuffmanTree(arr, freq, size+1);

    string tempString;
    encodeCharacters(root, tempString);

    for (const auto &item: codeMap) {
    cout << "{" << item.first << ": " << item.second << "}\n";
    }
}


void writeHeader(ofstream &outputStream) {
    for (const auto &item : codeMap)
        outputStream << item.first << CHARACTER_SEPARATOR << item.second << VALUE_SEPARATOR;
    outputStream << HEADER_SEPARATOR;
}

void compressTofile(string InputfileName ,string OutputfileName) {

    char character;
    string file;
    ifstream inputStream;
    ofstream outputStream;
    outputStream.open(OutputfileName, ios::out);
    inputStream.open(InputfileName, ios::in);
    writeHeader(outputStream);
    while (inputStream.get(character))
        file += codeMap[character];
    inputStream.close();
    file += codeMap[PSEUDO_EOF];
    unsigned long remainder = (file.size() - 1) % 8;
    for (int i = 0; i < 8 - remainder; ++i)
        file += '0';
    stringstream stringStream(file);

    while (stringStream.good()) {
        bitset<8> bits;
        stringStream >> bits;
        char c = char(bits.to_ulong());
        outputStream << c;
    }


    outputStream.flush();
    outputStream.close();
}

void readHeader(ifstream &inputStream) {
    codeMap.clear();
    char character;
    inputStream.get(character);
    char key = character;
    while (character != HEADER_SEPARATOR) {
        if (character == CHARACTER_SEPARATOR) {
            inputStream.get(character);
            while (character != VALUE_SEPARATOR) {
                codeMap[key] += character;
                inputStream.get(character);
            }
        } else
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
