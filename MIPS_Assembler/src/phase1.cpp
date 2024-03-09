#include "phase2.cpp"
#include <algorithm>

string RemoveComments(string line) {
    int found = line.find("#");
    if (found != -1) {
        return line.substr(0,found);
    }
    return line;
}

string isBinaryNumber(int n) {
    int binaryNum[28] = {0};
    string binary = "";
    int i = 0;
    while (n > 0 && i<=28) {
        binaryNum[i] = n % 2;
        n = n / 2;
        i++;
    }
    for (int j = 27; j >= 0; j--) {
        binary = binary + std::to_string(binaryNum[j]);
    }
    return binary;
}

string ProcessingLine(string line, int n) {
    string tmp;tmp.clear();
    for(auto v:line){
        if(v!=' '&&v!='\t')tmp.push_back(v);
    }
    line = tmp;
    line = RemoveComments(line);
    int found = line.find(":");
    if (found != -1) {
        string label = line.substr(0, found);
        labelTable[label] = {addTwoBinarys("0000010000000000000000000000", isBinaryNumber(n * 4)),to_string(n)};
        if (found + 1 == line.size()) {return "";}
        else {return line.substr(found + 1);}
    }
    return line;
}
