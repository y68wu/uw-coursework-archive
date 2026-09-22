#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <sstream>

using namespace std;

static vector<string> splitWS(const string &line) {
    vector<string> out;
    istringstream iss(line);
    string tok;
    while (iss >> tok) out.push_back(tok);
    return out;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    enum Section { NONE, CFG, INPUT, ACTIONS };
    Section sec = NONE;

    struct Rule { string lhs; vector<string> rhs; };
    vector<Rule> rules;
    vector<string> input_tokens;
    vector<pair<string,int>> actions; // {"shift"/"print"/"reduce", n}

    string line;
    auto switchSection = [&](const string &hdr){
        if      (hdr == ".CFG")      sec = CFG;
        else if (hdr == ".INPUT")    sec = INPUT;
        else if (hdr == ".ACTIONS")  sec = ACTIONS;
        else if (hdr == ".END")      sec = NONE; // loop will break when seen
        else                         sec = NONE;
    };

    while (getline(cin, line)) {
        // trim leading whitespace
        size_t p = line.find_first_not_of(" \t\r\n");
        if (p == string::npos) {
            // ignore blank lines in any section
            continue;
        }
        if (line[p] == '.') {
            string hdr = line.substr(p);
            if (hdr == ".END") break;
            switchSection(hdr);
            continue;
        }

        if (sec == CFG) {
            vector<string> toks = splitWS(line);
            if (toks.empty()) continue;
            Rule r;
            r.lhs = toks.front();
            if (!(toks.size() == 2 && toks[1] == ".EMPTY")) {
                r.rhs.assign(toks.begin() + 1, toks.end());
            }
            rules.push_back(r);
        } else if (sec == INPUT) {
            vector<string> toks = splitWS(line);
            input_tokens.insert(input_tokens.end(), toks.begin(), toks.end());
        } else if (sec == ACTIONS) {
            vector<string> toks = splitWS(line);
            if (toks.empty()) continue;
            if (toks[0] == "shift") {
                actions.push_back(make_pair(string("shift"), -1));
            } else if (toks[0] == "print") {
                actions.push_back(make_pair(string("print"), -1));
            } else if (toks[0] == "reduce" && toks.size() >= 2) {
                int n = 0;
                {
                    istringstream ss(toks[1]);
                    ss >> n;
                }
                actions.push_back(make_pair(string("reduce"), n));
            }
        }
    }

    deque<string> red;      // reduction sequence (right end = top)
    deque<string> inq;      // input sequence
    for (size_t i = 0; i < input_tokens.size(); ++i) inq.push_back(input_tokens[i]);

    auto do_print = [&](){
        if (!red.empty()) {
            for (size_t i = 0; i < red.size(); ++i) {
                if (i) cout << ' ';
                cout << red[i];
            }
            cout << " .";
        } else {
            cout << ".";
        }
        if (!inq.empty()) {
            cout << ' ';
            for (size_t i = 0; i < inq.size(); ++i) {
                if (i) cout << ' ';
                cout << inq[i];
            }
        }
        cout << '\n';
    };

    for (size_t i = 0; i < actions.size(); ++i) {
        const string &kind = actions[i].first;
        int arg = actions[i].second;

        if (kind == "shift") {
            if (!inq.empty()) {
                red.push_back(inq.front());
                inq.pop_front();
            }
        } else if (kind == "reduce") {
            if (arg >= 0 && arg < (int)rules.size()) {
                const Rule &r = rules[arg];
                size_t need = r.rhs.size();     // 0 for epsilon
                for (size_t k = 0; k < need && !red.empty(); ++k) {
                    red.pop_back();
                }
                red.push_back(r.lhs);
            }
        } else if (kind == "print") {
            do_print();
        }
    }
    return 0;
}
