#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <vector>
#include <tuple>

using namespace std;

const char BLANK = '⊔';
const int MAX_STEPS = 1000;

unordered_map<string, char> reverseEncoding = {{"00", '0'}, {"01", '1'}, {"10", '#'}, {"11", ';'}};

int safeBinaryToInt(const string& binary) {
    if (binary.empty()) return -1;
    for (char c : binary) {
        if (c != '0' && c != '1') return -1;
    }
    try {
        return stoi(binary, nullptr, 2);
    } catch (...) {
        return -1;
    }
}

string simulateTuringMachine(const string& transitionTable, const string& input) {
    string decoded;
    for (size_t i = 0; i < transitionTable.size(); i += 2) {
        if (i + 1 >= transitionTable.size()) return "Error";
        string pair = transitionTable.substr(i, 2);
        auto it = reverseEncoding.find(pair);
        if (it == reverseEncoding.end()) return "Error";
        decoded += it->second;
    }

    unordered_map<string, tuple<int, char, char>> transitions;
    stringstream ss(decoded);
    string trans;
    while (getline(ss, trans, ';')) {
        if (trans.empty()) continue;
        vector<string> parts;
        stringstream ts(trans);
        string part;
        while (getline(ts, part, '#'))
            parts.push_back(part);
        if (parts.size() != 5) continue;

        int fromState = safeBinaryToInt(parts[0]);
        int currSymIdx = safeBinaryToInt(parts[1]);
        int toState = safeBinaryToInt(parts[2]);
        int newSymIdx = safeBinaryToInt(parts[3]);
        string dirPart = parts[4];

        if (fromState == -1 || currSymIdx == -1 || toState == -1 || newSymIdx == -1 ||
            dirPart.size() != 1 || (dirPart[0] != '0' && dirPart[0] != '1'))
            return "Error";

        char currSym = (currSymIdx == 0) ? BLANK : (currSymIdx - 1 >= 0 && currSymIdx - 1 <= 9) ? '0' + (currSymIdx - 1) : '\0';
        char newSym = (newSymIdx == 0) ? BLANK : (newSymIdx - 1 >= 0 && newSymIdx - 1 <= 9) ? '0' + (newSymIdx - 1) : '\0';
        if (currSym == '\0' || newSym == '\0') return "Error";

        char dir = (dirPart[0] == '0') ? 'L' : 'R';
        transitions[to_string(fromState) + "_" + currSym] = {toState, newSym, dir};
    }

    string tape = string(1, BLANK) + input + string(1, BLANK);
    int head = 1, state = 0, step = 0;

    while (step++ < MAX_STEPS) {
        char currSym = tape[head];
        string key = to_string(state) + "_" + currSym;
        if (!transitions.count(key)) return "Error";

        auto [newState, newSym, dir] = transitions[key];
        tape[head] = newSym;
        state = newState;

        if (dir == 'L') {
            if (--head < 0) {
                tape = string(1, BLANK) + tape;
                head = 0;
            }
        } else {
            if (++head >= tape.size())
                tape += BLANK;
        }

        if (state == 1) return "Accept";
        if (state == 2) return "Reject";
    }

    return "Error";
}

int main() {
    string tt, input;
    getline(cin, tt);
    getline(cin, input);
    cout << simulateTuringMachine(tt, input) << endl;
    return 0;
}