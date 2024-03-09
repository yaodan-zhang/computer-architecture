#include <iostream>
#include <fstream>
#include "src/phase1.cpp"
using namespace std;

int main(int argc,char* argv[]) {
    if(argc < 3 or argc >= 4)
    {
        throw std::invalid_argument("Please enter a testfile and an expected output file! \n");
    }
    fstream testFile;
    testFile.open(argv[1], ios::in); //read file for the first time and process its lines
    ofstream processedFile("FileProcessing.txt");
    if (testFile.is_open() && processedFile.is_open()) { 
        string line;
        int n = 0;
        int start = 0;
        while (getline(testFile, line)) { 
            if (line.find(".text") != -1) {start = 1; continue;} //read from .text
            if (start == 1) {
                line = ProcessingLine(line, n); //discard comments and save labels
                if (line == "") {continue;}
                else {
                    processedFile << line << "\n";
                    ++n;
                }
            }
        }
    }
    testFile.close(); 
    processedFile.close();
    ofstream outputFile("output.txt"); 
    fstream sourceFile;                   //read the file for the second time
    sourceFile.open("FileProcessing.txt", ios::in);
    if (sourceFile.is_open() && outputFile.is_open()) {
        string line;
        int k = 0;
        while (getline(sourceFile, line)) {
            outputFile << GetMachineCode(line, k) << "\n";
            ++ k;
        }
    }
    sourceFile.close();
    outputFile.close();
    remove("FileProcessing.txt");
    fstream myOutput, expectedOutput;
    myOutput.open("output.txt", ios::in);
    expectedOutput.open(argv[2], ios::in);
    if (myOutput.is_open() && expectedOutput.is_open()) {
        string line1, line2;
        int res = 0;
        while (getline(myOutput, line1) && getline(expectedOutput, line2)) {
            if (line1.substr(0,32) != line2.substr(0,32)) {
                ++ res;
            }
        }
        if (res == 0) {cout << "Congrats output is correct !!";}
        else {cout << "Output is incorrect !";}
    }
    myOutput.close();
    expectedOutput.close();
}