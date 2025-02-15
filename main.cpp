#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <queue>

using namespace std;

const size_t chunk=100000;

int divideFile(const string& originalFile){
    ifstream inFile(originalFile);
    if(!inFile){
        cout<<"Error al abrir el archivo "<<originalFile<<endl;
        return -1;
    }
    vector<string> lines;
    string line;
    int fileCounter=0;

    while(getline(inFile, line)){
        lines.push_back(line);

        if(lines.size()>=chunk){
            sort(lines.begin(), lines.end());

            ofstream testFile("test"+to_string(fileCounter)+".txt");
            for(const auto& l:lines){
                testFile<<l<<"\n";
            }
            testFile.close();
            lines.clear();
            fileCounter++;
        }
    }
    if(!lines.empty()){
        sort(lines.begin(), lines.end());
        ofstream testFile("test"+to_string(fileCounter)+".txt");
        for(const auto& l:lines){
            testFile<<l<<"\n";
        }
        testFile.close();
        fileCounter++;
    }
    inFile.close();
    return fileCounter;
}

struct FileLine{
    string line;
    ifstream* file;

    bool operator>(const FileLine& other) const{
        return line> other.line;
    }
};

void mergeFiles(int numFiles, const string& outputFile){
    priority_queue<FileLine, vector<FileLine>, greater<FileLine>> minHeap;
    vector<ifstream> fileStreams(numFiles);

    for(int i=0; i<numFiles; i++){
        fileStreams[i].open("test"+to_string(i)+".txt");
        if(fileStreams[i]){
            string line;
            if(getline(fileStreams[i], line)){
                minHeap.push({line, &fileStreams[i]});
            }
        }
    }
    ofstream outFile(outputFile);
    while(!minHeap.empty()){
        FileLine smallest=minHeap.top();
        minHeap.pop();

        outFile<<smallest.line<<"\n";

        string nextLine;
        if(getline(*smallest.file, nextLine)){
            minHeap.push({nextLine, smallest.file});
        }
    }

    outFile.close();
    for(auto& fs:fileStreams) fs.close();
}

int main(){
    string fileName="data.txt";
    string outputFile="sorted_data.txt";

    int numFiles=divideFile(fileName);
    if(numFiles>0){
        mergeFiles(numFiles, outputFile);
        cout<<"Archivo ordenado guardado en: "<<outputFile<<endl;
    } else{
        cout<<"Error al abrir el archivo"<<endl;
    }
    return 0;
}