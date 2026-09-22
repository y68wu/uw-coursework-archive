#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <set>
#include <map>
using namespace std;

const string ID = "ID";
const string INT = "INT";
const string NUM = "NUM";
const string NUL = "NULL";
const string DCL = "dcl";
const string DCLS = "dcls";
const string EXPR = "expr";
const string TERM = "term";
const string FACTOR = "factor";


struct TreeNode {
    string line;
    vector<string> tokens;
    vector<TreeNode*> children;
    string type;
};


const map<string, string> name_to_type = {
    {"NUM", "int"},
    {"INT", "int"},
    {"INT STAR", "int*"},
    {"NULL", "int*"}
};

TreeNode* rootNode = nullptr;

vector<string> tokenize(const string &input) {
    istringstream iss(input);
    vector<string> tokens;
    string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}


set<string> nonterminals = {"start", "procedures", "main", "dcl", "type", "dcls", "statements", "expr", "term", "factor", "lvalue"};

set<string> terminals =  { "BOF", "INT", "WAIN", "LPAREN", "RPAREN", "COMMA", 
    "LBRACE", "RBRACE", "RETURN", "SEMI", "STAR", 
    "ID", "NUM", "NULL", "EOF", "BECOMES",
    "PLUS", "MINUS", "SLASH", "PCT", "AMP", "NEW", 
    "LBRACK", "RBRACK" };



TreeNode* parseTree() {
    string line;
    if (!getline(cin, line)) {
        return nullptr;
    }
    
    TreeNode* node = new TreeNode();
    node->line = line;
    node->tokens = tokenize(line);
    
    if (nonterminals.count(node->tokens[0])) {
        if (node->tokens.size() == 2 && node->tokens[1] == ".EMPTY") {
            return node;
        }
        
        int num_symbols = node->tokens.size() - 1;
        for (int i = 0; i < num_symbols; i++) {
            TreeNode* child = parseTree();
            if (child) {
                node->children.push_back(child);
            }
        }
    }
    return node;
}

void cleanup_tree(TreeNode* node) {
    if (!node) {
        return;
    }
    
    for (TreeNode* child : node->children) {
        cleanup_tree(child);
    }
    delete node;
}

string get_word(TreeNode* node) {
    if (node->tokens.size() >= 2) {
        return node->tokens[1];
    }
    return "";
}

void format_error() {
    if (rootNode) { 
        cleanup_tree(rootNode); 
        rootNode = nullptr; 
    }
    cerr << "ERROR" << endl;
    exit(0);
}

TreeNode* check_main(TreeNode* root) {
    if (!root || root->tokens.empty()) {
        return nullptr;
    }
    
    if (root->tokens[0] == "start" && root->children.size() >= 3) {
        TreeNode* procedures = root->children[1];
        if (!procedures->children.empty()) {
            return procedures->children[0];
        }
    }
    return nullptr;
}

string get_type(TreeNode* node) {
    if (node->children.empty()) {
        string kind = node->tokens[0];
        if (kind == "NUM") {
            return "int";
        } else if (kind == "NULL") {
            format_error();
        } else if (kind == "ID") {
            string param = node->tokens[1];
            if (symboltable.find(param) == symboltable.end()) {
                format_error();
                return symboltable[param];
            }
        } else {
            return "";
        }
    } else {
        string label = node->tokens[0];
        if (label == "expr" || label == "term") {
            if (node->children.empty()) {
                return "";
            }
            return get_type(node->children[0]);
        }
        
        else if (label == "factor") {
            if (node->children.empty()) {
                return "";
            }
            return get_type(node->children[0]);
        }

        else if 
            if (node->children.size() == 3) {
                string one = get_type(node->children[0]);
                string two = node->children[1]->tokens[0];
                string three = get_type(node->children[2]);

                if (two == "STAR" || two == "SLASH" || two == "PCT") {
                    if (one == "int"  && three == "int") {return "int";}
                    else (format_error());
                }
            }
            return get_type(node->children[0]);
        }
        else if (label == "factor") {
            if (node->tokens[0] == "NEW") {
                if(get_type(node->children[3]) != "int") {format_error();}
                else if (2 == node->children.size()) {
                    string one = get_type(node->children[1]);
                    string zero = node->children[0]->tokens[0];
                    if (one == "int*" && zero == "STAR") {return "int";}
                    else if (one == "int" && zero == "AMP") return "int*";
                }
            }
            return get_type(node->children[0]);
        }
          
    }
    return "int";
}

void annotate_tree(TreeNode* node) {
    if (node->tokens[0] == "ID") {
        if (symboltable.find(node->tokens[1]) != symboltable.end()) {
            node->line += " : " + symboltable[node->tokens[1]];
        }
    } else if (node->tokens[0] == "NUM") {
        node->line += " : int";
    } else if (node->tokens[0] == "NULL") {
        node->line += " : int*";
    } else if (node->tokens[0] == "expr" || node->tokens[0] == "term" || node->tokens[0] == "factor") {
        node->line += " : " + get_type(node);
    }

    for (TreeNode* child : node->children) {
        annotate_tree(child);
    }
}


void print_tree(TreeNode* node) {
    if (!node) {
        return;
    }
    
    cout << node->line << endl;
    
    for (TreeNode* child : node->children) {
        print_tree(child);
    }
}

int main() {
    TreeNode* root = parseTree();
    rootNode = root;
    
    if (!root) {
        return 0;
    }
    
    TreeNode* mainNode = check_main(root);
    if (!mainNode) {
        format_error();
    }
    
    if (mainNode->children.size() < 14) {
        format_error();
    }
    
    TreeNode* dcl1 = mainNode->children[3];
    TreeNode* dcl2 = mainNode->children[5];
    
    if (dcl1->children.size() < 2 || dcl2->children.size() < 2) {
        format_error();
    }
    
    string param1 = get_word(dcl1->children[1]);
    string param2 = get_word(dcl2->children[1]);
    
    if (param1 == param2) {
        format_error();
    }
    
    TreeNode* typeNode = dcl2->children[0];
    if (typeNode->tokens.size() != 2 || typeNode->tokens[1] != "INT") {
        format_error();
    }
    
    
    
    expand_tree(root);
    print_tree(root);
    
    cleanup_tree(root);
    rootNode = nullptr;
    return 0;
}