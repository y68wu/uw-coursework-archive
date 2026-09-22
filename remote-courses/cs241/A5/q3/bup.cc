#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <sstream>

using std::cin;
using std::cout;
using std::deque;
using std::size_t;
using std::string;
using std::vector;

struct Rule {
    string lhs;
    size_t rhs_len; // 0 if .EMPTY
};

enum Section { SEC_OTHER, SEC_CFG, SEC_INPUT, SEC_ACTIONS };

static inline bool is_header(const string &s) {
    return !s.empty() && s[0] == '.';
}

static inline void print_progress(const deque<string> &red, const deque<string> &inp) {
    if (!red.empty()) {
        for (size_t i = 0; i < red.size(); ++i) {
            if (i) cout << ' ';
            cout << red[i];
        }
        cout << " .";
    } else {
        cout << ".";
    }
    if (!inp.empty()) {
        cout << ' ';
        for (size_t i = 0; i < inp.size(); ++i) {
            if (i) cout << ' ';
            cout << inp[i];
        }
    }
    cout << '\n';
}

int main() {
    // no custom allocs; let STL clean up automatically
    Section sec = SEC_OTHER;

    vector<Rule> rules;          rules.reserve(256);
    deque<string> input; 
    vector<string> actions;      actions.reserve(512);

    string line;
    while (std::getline(cin, line)) {
        if (line.empty()) continue;

        if (is_header(line)) {
            if      (line == ".CFG")      sec = SEC_CFG;
            else if (line == ".INPUT")    sec = SEC_INPUT;
            else if (line == ".ACTIONS")  sec = SEC_ACTIONS;
            else if (line == ".END")      break;
            else                           sec = SEC_OTHER;
            continue;
        }

        std::istringstream iss(line);

        if (sec == SEC_CFG) {
            Rule r; 
            if (!(iss >> r.lhs)) continue;
            string tok; 
            size_t count = 0;
            while (iss >> tok) {
                if (tok == ".EMPTY") { count = 0; break; }
                ++count;
            }
            r.rhs_len = count;
            rules.push_back(r);
        }
        else if (sec == SEC_INPUT) {
            string tok;
            while (iss >> tok) input.push_back(tok);
        }
        else if (sec == SEC_ACTIONS) {
            string a; 
            iss >> a;
            if (a == "shift" || a == "print") {
                actions.push_back(a);
            } else if (a == "reduce") {
                string n; iss >> n;
                actions.push_back(a + " " + n); // store as a single string
            }
        }
        // ignore any lines in SEC_OTHER
    }

    // simulate
    vector<string> red; red.reserve(512);
    for (size_t i = 0; i < actions.size(); ++i) {
        const string &act = actions[i];

        if (act == "shift") {
            if (!input.empty()) {
                red.push_back(input.front());
                input.pop_front();
            }
        }
        else if (act == "print") {
            // copy red -> deque view for printing without reallocations
            deque<string> rview(red.begin(), red.end());
            print_progress(rview, input);
        }
        else if (act.size() >= 6 && act.compare(0,6,"reduce") == 0) {
            // parse "reduce N"
            std::istringstream iss(act);
            string drop; int n = 0;
            iss >> drop >> n;
            if (n >= 0 && n < (int)rules.size()) {
                size_t need = rules[(size_t)n].rhs_len;
                while (need > 0 && !red.empty()) { red.pop_back(); --need; }
                red.push_back(rules[(size_t)n].lhs);
            }
        }
    }
    return 0;
}
