#include <string>
#include <map>
#include <vector>
using namespace std;

std::map<string, vector<string>> labelTable; 

vector<string> findValue(string key) { 
    for(std::map<string,vector<string>>::iterator iter = labelTable.begin(); iter != labelTable.end(); iter++) {
        if (iter->first == key) {return iter->second;}
    }
    return {NULL, NULL};
}