#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <time.h>
#include "MinHeapNode.h"
#include "huffman.cpp"
#include "freqTable.cpp"
using namespace std;
//extern unordered_map<char,string> codeMap;//codeMap created in MinHeap and used in huffman

int filesize(const char* filename)
{
    ifstream in_file(filename, ios::binary);
   in_file.seekg(0, ios::end);
   int file_size = in_file.tellg();
   return file_size;
}

int main(){
    unordered_map<char, int> frequencyMap;
    string workingMode;
    cout<<"Enter working mode: ";
    cin >> workingMode;

    if(workingMode == "compress")
    {
        clock_t tStart = clock();
        frequencyMap = frequencyTable("FileToRead.txt");
        createcodeMap(frequencyMap);
        compressTofile("FileToRead.txt","compressed.txt");
        cout <<"Time taken: "<<(1.0*(clock() - tStart)/CLOCKS_PER_SEC)<<"sec"<<endl;
        cout << "Input File Size : "<<filesize("FileToRead.txt")<<" bytes."<<endl;
        cout<< "Compressed File Size : "<<filesize("compressed.txt")<<" bytes."<<endl;
        cout<< "Compression Ratio : "<<(1.0*filesize("compressed.txt")/filesize("FileToRead.txt"))<<endl;
    }
    else if(workingMode == "decompress")
    {
        clock_t tStart = clock();
        frequencyMap = frequencyTable("FileToRead.txt");
        createcodeMap(frequencyMap);
        deHuffer("compressed.txt","decompressed.txt");
        cout <<"Time taken: "<<(1.0*(clock() - tStart)/CLOCKS_PER_SEC)<<"sec"<<endl;
        cout << "Input File (Compressed) Size : "<<filesize("compressed.txt")<<" bytes."<<endl;
        cout<< "DeCompressed File Size : "<<filesize("decompressed.txt")<<" bytes."<<endl;
    }
    return 0;
}



