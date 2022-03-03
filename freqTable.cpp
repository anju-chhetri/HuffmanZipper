#include <iostream>
#include <unordered_map>
#include <string>
using namespace std;

unordered_map<char, int> FrequencyTable(string text){
    unordered_map<char, int> frequency;
    for(char c: text){
        frequency[c]++;
    }
    return frequency;
}
