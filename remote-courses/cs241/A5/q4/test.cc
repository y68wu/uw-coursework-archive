#include<iostream>
#include<vector>
#include<deque>
#include<string>
#include<sstream>
#include<fstream>
#include<map>
using namespace std;
struct Rule{
    string left;
    vector<string> right;
};

void myprint(const vector<string>& symbolS, const deque<string>& input) {
    
    for (auto& s:symbolS) {
        cout << s;
        cout <<" ";
    }
    cout << ".";
    for (auto & i : input) {
        cout << " " << i;
    }
    cout << "\n";
}

enum Compo{CFG, INPUT, TRANSITIONS, REDUCTIONS, Other};

int main(int argc, char* argv[]) {
    istream* in = &std::cin;
    ifstream file;

    map<pair<int, string>, int> trans; // now , symbol, next
    map<int, vector<pair<int, string>>> reductions; // now rule, tag

    if (argc > 1) {
        file.open(argv[1]);
        if (file.is_open()) {
            in = &file;
        }
    }


    vector<Rule> CFG_rules;
    deque<string> input;

    Compo cur = Other;
    string line;

    while (std::getline(*in, line)) {
        if (line.empty()){continue;}

        if(line[0] =='.') {
            if(line == ".CFG") {
                cur = CFG;
            } else if(line == ".INPUT"){
                cur = INPUT;
            } else if(line == ".TRANSITIONS"){
                cur = TRANSITIONS;
            } else if (line == ".REDUCTIONS"){
                cur = REDUCTIONS;
            } else if(line == ".END"){
                break; 
            }
            continue;
        }
        istringstream iss(line);
        if (cur == CFG) {
            Rule rule;
            iss >> rule.left;
            string word;
            while (iss >> word) {
                if (word == ".EMPTY") {
                    continue;
                } else {
                    rule.right.push_back(word);
                }
            }
            CFG_rules.push_back(rule);
        } else if (cur == INPUT){
            string word;
            while(iss >> word) {
                input.push_back(word);
            }
        } else if (cur == TRANSITIONS) {
            int from;
            int to;
            string word;
            iss >> from >> word >> to;
            trans[{from, word}] =to;
        } else if (cur == REDUCTIONS) {
            int stateN;
            int ruleN;
            string tag;
            iss >> stateN >> ruleN >> tag;
            reductions[stateN].push_back({ruleN, tag});
        }
    }
            
    vector<int> stateV;
    vector<string> symoblV;
    stateV.push_back(0);
    int k = 0;

    myprint(symoblV, input);

    while(1) {
        int cur = stateV.back(); // sur state
        string next = (input.empty()? "EOF" : input.front());

        ; // find the next stage
        if (trans.find({cur, next}) != trans.end()) {
            // d othe trnlation
            int nextstate = trans[{cur,next}];
            k++;
            string added = input.front();
            input.pop_front();
            symoblV.push_back(added); // right get in
            stateV.push_back(nextstate);
            myprint(symoblV,input);
            continue;
        }
        // reduction  
        if (reductions.find(cur) != reductions.end()) {
            // find
            bool find =0;
            for (auto& potential : reductions[cur]) {
                int ruleN = potential.first;
                string the_tag = potential.second;
                if(the_tag == next ||(the_tag == ".ACCEPT" && next == "EOF")) {
                    // the right ruleN here
                    find = 1;
                    Rule rule = CFG_rules[ruleN];
                    for(size_t i = 0; i < rule.right.size(); ++i){
                        if(!symoblV.empty()) {
                            symoblV.pop_back();
                            // right, get out
                        }
                        if(!stateV.empty()) {
                            stateV.pop_back();
                        }
                    }// 
                    symoblV.push_back(rule.left);//  get back

                    if (the_tag == ".ACCEPT") {
                        myprint(symoblV, input);
                        return 0;
                    }
                    int newstate = stateV.back();
                    auto this_to = trans.find({newstate, rule.left});
                    if (this_to!= trans.end()){
                        stateV.push_back(this_to->second);
                        myprint(symoblV,input);
                    } else {
                        cerr << "ERROR at "<< (k+1) << "\n";    // 10 EOF 5
                        return 1;
                    }
                    break;
                }
            }
            if (find) { continue;}
        } 
        cerr << "ERROR at "<< (k+ 1) << "\n";
        return 1;
        
    }
    return 0;
}
