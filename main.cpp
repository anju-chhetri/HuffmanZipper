#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include "huffman.cpp"
#include "freqTable.cpp"
using namespace std;

int main(){
//     fstream file;
//     file.open("FileToRead.txt",ios::in);
//     string text;
//     file>>text;
    ifstream file("FileToRead.txt");
    string text((istreambuf_iterator<char>(file) ),
                       (istreambuf_iterator<char>()));
    unordered_map<char, int> textRead = FrequencyTable(text);
    huffmans(textRead);
    file.close();
}

// struct Node{
//     char character;
//     int frequency;
//     struct Node *leftChild, *rightChild;
// };
// typedef struct Node node;
//
// node *leafNodeAssign(char ch, int freq){
//     node *temp =  (node*)malloc(sizeof(Node));
//     temp->character = ch;
//     temp->frequency = freq;
//     temp->leftChild = NULL; temp->rightChild = NULL;
//     return temp;
// }
//
//
//
// int main(){
//     char test[5] = {'a', 'b','c','d','e'};
//     int freq[5] = {10,45,34,23,45};
//     node nodearr[5];
//     for(int i=0;i<5;i++){
//         nodearr[i] = leafNodeAssign(test[i],freq[i]);
//     }
//
//
//}

