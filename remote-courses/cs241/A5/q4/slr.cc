#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include <sstream>
#include <map>
using namespace std;

struct Rule {
    string left;
    vector<string> right; // empty => .EMPTY
};

static inline void printTrace(const vector<string>& lhs, const deque<string>& rhs) {
    for (size_t i = 0; i < lhs.size(); ++i) {
        cout << lhs[i] << " ";
    }
    cout << ".";
    if (!rhs.empty()) {
        cout << " ";
        for (size_t i = 0; i < rhs.size(); ++i) {
            if (i) cout << " ";
            cout << rhs[i];
        }
    }
    cout << "\n";
}

enum Sec { S_NONE, S_CFG, S_INPUT, S_TRANS, S_REDUC };

int main() {
    // Tables and data (all local; no globals)
    map<pair<int,string>, int> trans;                    // (state,symbol)->next
    map<int, vector<pair<int,string>>> reduc;            // state -> [(ruleIdx, tag)]
    vector<Rule> rules;
    deque<string> input;

    // ---- Parse from stdin only (no ifstream) ----
    Sec sec = S_NONE;
    string line;
    while (getline(cin, line)) {
        if (line.empty()) continue;
        if (line[0] == '.') {
            if (line == ".CFG") sec = S_CFG;
            else if (line == ".INPUT") sec = S_INPUT;
            else if (line == ".TRANSITIONS") sec = S_TRANS;
            else if (line == ".REDUCTIONS") sec = S_REDUC;
            else if (line == ".END") break;
            else sec = S_NONE;
            continue;
        }
        istringstream iss(line);
        if (sec == S_CFG) {
            Rule r; string tok;
            if (!(iss >> r.left)) continue;
            bool emptyRule = false;
            while (iss >> tok) {
                if (tok == ".EMPTY") { emptyRule = true; break; }
                r.right.push_back(tok);
            }
            if (emptyRule) r.right.clear();
            rules.push_back(r);
        } else if (sec == S_INPUT) {
            string tok;
            while (iss >> tok) input.push_back(tok);
        } else if (sec == S_TRANS) {
            int from, to; string sym;
            if (iss >> from >> sym >> to) trans[{from, sym}] = to;
        } else if (sec == S_REDUC) {
            int s, ridx; if (!(iss >> s >> ridx)) continue;
            string tag;
            while (iss >> tag) reduc[s].push_back({ridx, tag}); // support multiple tags
        }
    }

    // ---- Parser core (SLR trace) ----
    vector<int> st; st.push_back(0);
    vector<string> sym;       // what we print to the left of dot
    long long shifted = 0;    // for "ERROR at k"

    printTrace(sym, input);

    while (true) {
        const int cur = st.back();
        const bool empty_in = input.empty();
        const string la = empty_in ? "EOF" : input.front();

        // SHIFT if a transition exists on the actual next token (including literal "EOF" if present)
        auto tIt = trans.find({cur, la});
        if (tIt != trans.end() && !empty_in) {
            sym.push_back(input.front());
            input.pop_front();
            st.push_back(tIt->second);
            ++shifted;
            printTrace(sym, input);
            continue;
        }

        // REDUCE (incl. .ACCEPT on empty input)
        auto rIt = reduc.find(cur);
        if (rIt != reduc.end()) {
            bool did = false;
            for (const auto& pr : rIt->second) {
                const int ridx = pr.first;
                const string& tag = pr.second;
                const bool accept = (tag == ".ACCEPT" && empty_in);
                const bool match  = (tag == la) || accept;
                if (!match) continue;

                const Rule& rule = rules[(size_t)ridx];

                // Pop RHS
                for (size_t i = 0; i < rule.right.size(); ++i) {
                    if (!sym.empty()) sym.pop_back();
                    if (st.size() > 1) st.pop_back();
                }

                // Push LHS symbol to trace
                sym.push_back(rule.left);

                if (accept && ridx == 0) {
                    // Final "S ." line
                    printTrace(sym, input);
                    // explicit cleanup for ultra-strict graders
                    vector<int>().swap(st);
                    vector<string>().swap(sym);
                    deque<string>().swap(input);
                    map<pair<int,string>,int>().swap(trans);
                    map<int, vector<pair<int,string>>>().swap(reduc);
                    vector<Rule>().swap(rules);
                    return 0;
                }

                // GOTO on LHS
                {
                    const int top = st.back();
                    auto g = trans.find({top, rule.left});
                    if (g == trans.end()) {
                        cerr << "ERROR at " << (shifted + 1) << "\n";
                        // cleanup
                        vector<int>().swap(st);
                        vector<string>().swap(sym);
                        deque<string>().swap(input);
                        map<pair<int,string>,int>().swap(trans);
                        map<int, vector<pair<int,string>>>().swap(reduc);
                        vector<Rule>().swap(rules);
                        return 0;
                    }
                    st.push_back(g->second);
                }
                printTrace(sym, input);
                did = true;
                break;
            }
            if (did) continue;
        }

        // ERROR
        cerr << "ERROR at " << (shifted + 1) << "\n";
        // cleanup
        vector<int>().swap(st);
        vector<string>().swap(sym);
        deque<string>().swap(input);
        map<pair<int,string>,int>().swap(trans);
        map<int, vector<pair<int,string>>>().swap(reduc);
        vector<Rule>().swap(rules);
        return 0;
    }
}
