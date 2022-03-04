#include <iostream>
#include <unordered_map>
#include <string>
using namespace std;

unordered_map<char, int> frequencyTable(string fileName){
    char c;
    unordered_map<char, int> frequencyMap;
    ifstream inputFile;

    inputFile.open(fileName, ios::in);

    while(inputFile.get(c))
        frequencyMap[c]++;
    return frequencyMap;
}
