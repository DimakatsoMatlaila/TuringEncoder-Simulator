#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>

using namespace std;

int main() {
    //Declare variables//
    string table, x;
    //cin>>table;
    //cin>>x;
    getline(cin, table);
    getline(cin, x);
    unordered_map<char, string> mapping;
    istringstream stream(table);
    string pair;

    //----------------Code Here---------------------//
    while (getline(stream, pair, '|')) {
        if (pair.empty()==false) {
            char t;
            string Val;
            if (pair[0] == ' ' && pair[1] == '=') {
                t = ' ';
                Val = pair.substr(2);
            } else {
                t = pair[0];
                Val = pair.substr(2);
            }
            mapping[t] = Val;
        }
    }

    for (char i : x) {
        if (mapping.find(i) != mapping.end()) {
            cout << mapping[i];
        }
    }

    cout << endl;
    return 0;
}
