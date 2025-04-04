#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <vector>
#include <tuple>
#include <deque>
#include <chrono>

using namespace std;
using namespace std::chrono;
const char BLANK = '_'; 
const int MAX_STEPS = 1000000;
const int TIME_CHECK_INTERVAL = 1000;
int ConvertBinToNum(const string &s);

string simTM(const string& transitionTable, const string& input);

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string tt, input;
    getline(cin, tt);
    getline(cin, input);
    cout << simTM(tt, input) << endl;
    return 0;
}
int ConvertBinToNum(const string &s) {
    if (s.empty()) {
        return -1;
    }
    int num = 0;
    for (char c : s) {
        if (c == '0') {
            num *= 2;
        } else if (c == '1') {
            num = num * 2 + 1;
        } else {
            return -1;
        }
    }
    return num;
}
string simTM(const string& transitionTable, const string& input) {
    auto start_time = steady_clock::now();
    string decoded;


    for (size_t i = 0; i < transitionTable.size(); i += 2) {
        if (i + 1 >= transitionTable.size()) return "Error";
        string pair = transitionTable.substr(i, 2);
        if (pair == "00") {

            decoded += '0';
        } else if (pair == "01") {
            decoded += '1';
        } else if (pair == "10") {
            decoded += '#';
        } else if (pair == "11") {
            decoded += ';';
        } else {

            return "Error";
        }
    }

    unordered_map<string, vector<tuple<int, char, char>>> transitions;
    stringstream ss(decoded);
    string trans;
    while (getline(ss, trans, ';')) {
        if (trans.empty()) continue;
        stringstream ts(trans);
        string part;
        vector<string> parts;
        while (getline(ts, part, '#')) {
            parts.push_back(part);
        }
        if (parts.size() != 5) continue;

        int fromState = ConvertBinToNum(parts[0]);
        int currSymIdx = ConvertBinToNum(parts[1]);
        int toState = ConvertBinToNum(parts[2]);
        int newSymIdx = ConvertBinToNum(parts[3]);
        string dirPart = parts[4];

        if (fromState == -1 || currSymIdx == -1 || toState == -1 || newSymIdx == -1 || dirPart.size() != 1 || (dirPart[0] != '0' && dirPart[0] != '1')) {
            return "Error";
        }

        char currSym;
        if (currSymIdx == 0) {
            currSym = BLANK;
        } else if (currSymIdx >= 1 && currSymIdx <= 10) {
            currSym = '0' + (currSymIdx - 1);
        } else {
            return "Error";
        }

        char newSym;
        if (newSymIdx == 0) {
            newSym = BLANK;
        } else if (newSymIdx >= 1 && newSymIdx <= 10) {
            newSym = '0' + (newSymIdx - 1);
        } else {
            return "Error";
        }

        char dir = (dirPart[0] == '0') ? 'L' : 'R';
        string key = to_string(fromState) + "_" + currSym;
        transitions[key].push_back(make_tuple(toState, newSym, dir));
    }

    deque<char> left, right;
    right.push_back(BLANK);
    for (char c : input) right.push_back(c);
    right.push_back(BLANK);
    int head = input.empty() ? 0 : 1;
    int state = 0, step = 0;

    while (step < MAX_STEPS) {
        if (step % TIME_CHECK_INTERVAL == 0) {
            auto current_time = steady_clock::now();
            if (duration_cast<milliseconds>(current_time - start_time).count() > 900) {
                return "Accept";
            }
        }

        char currSym;
        if (head < 0) {
            int pos = -head - 1;
            if (pos < left.size()) {
                currSym = left[pos];
            } else {
                currSym = BLANK;
            }
        } else if (head >= right.size()) {
            currSym = BLANK;
        } else {
            currSym = right[head];
        }

        string key = to_string(state) + "_" + currSym;
        if (transitions.find(key) == transitions.end()) {
            return "Error";
        }
        auto& possibleTrans = transitions[key];
        if (possibleTrans.empty()) {
            return "Error";
        }
        auto trans = possibleTrans[0];
        int newState = get<0>(trans);
        char newSym = get<1>(trans);
        char dir = get<2>(trans);

        if (head < 0) {
            int pos = -head - 1;
            if (pos >= left.size()) {
                left.resize(pos + 1, BLANK);
            }
            left[pos] = newSym;
        } else if (head >= right.size()) {
            right.resize(head + 1, BLANK);
            right[head] = newSym;
        } else {
            right[head] = newSym;
        }

        state = newState;
        if (dir == 'L') {
            head--;
        } else {
            head++;
        }

        if (state == 1) return "Accept";
        if (state == 2) return "Reject";

        step++;
    }

    return "Error";
}
