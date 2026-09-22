#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <cstdlib>
#include <algorithm>
using namespace std;

const string EMPTY = ".EMPTY";
const string ID = "ID";
const string INT = "INT";
const string STAR = "STAR";
const string NUM = "NUM";
const string NIL = "NULL";
const string DCL = "dcl";
const string DCLS = "dcls";
const string EXPR = "expr";
const string TERM = "term";
const string FACTOR = "factor";
const string LVALUE = "lvalue";
const string WAIN = "WAIN";
const string BECOMES = "BECOMES";
const string SEMI = "SEMI";
const string RETURN = "RETURN";
const string COMMA = "COMMA";
const string LPAREN = "LPAREN";
const string RPAREN = "RPAREN";
const string MAIN = "main";
const string START = "start";
const string PROCEDURES = "procedures";
const string TYPE = "type";


struct Node {
    string line;
    vector<string> tokens;
    vector<Node*> children;
    string type;
    
    ~Node() {
        for (Node* child : children) {
            delete child;
        }
        children.clear();
    }
};

map<string, string> symbolTable;

vector<string> terminals = {
    "BOF", "INT", "WAIN", "LPAREN", "RPAREN", "COMMA",
    "LBRACE", "RBRACE", "RETURN", "SEMI", "STAR",
    "ID", "NUM", "NULL", "EOF", "BECOMES"
};

vector<string> tokenize(const string &s) {
    istringstream iss(s);
    vector<string> tokens;
    string token;
    while (iss >> token)
        tokens.push_back(token);
    return tokens;
}

Node* parse_tree() {
    string line;
    if (!getline(cin, line)) return nullptr;
    
    Node* node = new Node();
    node->line = line;
    node->tokens = tokenize(line);
    
    if (find(terminals.begin(), terminals.end(), node->tokens[0]) == terminals.end()) {
        if (node->tokens.size() == 2 && node->tokens[1] == ".EMPTY") return node;
        

        for (size_t i = 1; i < node->tokens.size(); i++) {
            Node* child = parse_tree();
            if (child) node->children.push_back(child);
        }
    }
    return node;
}

void format_error(Node* root = nullptr) {
    cerr << "ERROR" << endl;
    delete root;
    exit(0);
}



string get_node_type(Node* typeNode) {
    if (!typeNode || typeNode->tokens[0] != TYPE) return "";
    
    if (typeNode->children.size() == 1 && typeNode->children[0]->tokens[0] == INT) {
        return "int";
    } else if (typeNode->children.size() == 2 && typeNode->children[0]->tokens[0] == INT && 
               typeNode->children[1]->tokens[0] == STAR) {
        return "int*";
    }
    
    return "";
}

pair<string, string> get_dcls(Node* dclNode) {
    if (!dclNode || dclNode->tokens[0] != DCL || dclNode->children.size() < 2) 
        return {"", ""};
    
    string type = get_node_type(dclNode->children[0]);
    string name = "";
    
    if (dclNode->children[1]->tokens[0] == ID) {
        name = dclNode->children[1]->tokens[1];
    }
    
    return {type, name};
}

void process_decls(Node* wainNode, Node* root) {
    symbolTable.clear();
    
    if (!wainNode || wainNode->tokens[0] != MAIN || wainNode->children.size() < 12) {
        format_error(root);
    }
    
    Node* param1 = wainNode->children[3];
    Node* param2 = wainNode->children[5];
    
    auto [param1Type, param1Name] = get_dcls(param1);
    auto [param2Type, param2Name] = get_dcls(param2);
    
    if (param1Name.empty() || param2Name.empty()) {
        format_error(root);
    }
    
    if (param1Name == param2Name) {
        format_error(root);
    }
    

    if (param2Type != "int") {
        format_error(root);
    }
    
    symbolTable[param1Name] = param1Type;
    symbolTable[param2Name] = param2Type;
    
    Node* dclsNode = wainNode->children[8];

    while (dclsNode && dclsNode->tokens[0] == DCLS && !dclsNode->children.empty()) {

        if (dclsNode->children.size() >= 5) {
            Node* dclNode = dclsNode->children[1];
            Node* initValueNode = dclsNode->children[3];
            
            auto [varType, varName] = get_dcls(dclNode);
            
            if (varName.empty()) {
                format_error(root);
            }
            if (symbolTable.find(varName) != symbolTable.end()) {
                format_error(root);
            }
            
            string initType = "";
            if (initValueNode->tokens[0] == NUM) {
                initType = "int";
            } else if (initValueNode->tokens[0] == NIL) {
                initType = "int*";
            } else {
                format_error(root);
            }
            
            if (varType != initType) {
                format_error(root);
            }
            
            symbolTable[varName] = varType;
            
            dclsNode = dclsNode->children[0];
        } else {
            break;
        }
    }
}

string check_type(Node* node, Node* root) {
    if (!node) return "";
    
    string nodeKind = node->tokens[0];
    
    if (nodeKind == NUM) {
        return "int";
    } else if (nodeKind == NIL) {
        return "int*";
    } else if (nodeKind == ID) {
        string varName = node->tokens[1];
        if (symbolTable.find(varName) == symbolTable.end()) {
            format_error(root);
        }
        return symbolTable[varName];
    }
    
    if (nodeKind == EXPR || nodeKind == TERM) {
        if (!node->children.empty()) {
            return check_type(node->children[0], root);
        }
    } else if (nodeKind == FACTOR) {
        if (node->children.size() == 1) {
            return check_type(node->children[0], root);
        } else if (node->children.size() == 3 && 
                   node->children[0]->tokens[0] == LPAREN && 
                   node->children[2]->tokens[0] == RPAREN) {
            return check_type(node->children[1], root);
        }
    }
    
    format_error(root);
    return "";
}

void check_return(Node* wainNode, Node* root) {
    if (wainNode->children.size() < 12) {
        format_error(root);
    }
    
    Node* returnExpr = nullptr;
    for (size_t i = 0; i < wainNode->children.size(); i++) {
        if (wainNode->children[i]->tokens[0] == RETURN && i + 1 < wainNode->children.size()) {
            returnExpr = wainNode->children[i + 1];
            break;
        }
    }
    
    if (!returnExpr) {
        format_error(root);
    }
    
    string returnType = check_type(returnExpr, root);
    if (returnType != "int") {
        format_error(root);
    }
}

void annotate_tree(Node* node, Node* root) {
    if (!node) return;
    
    string nodeKind = node->tokens[0];
    if (nodeKind == EXPR || nodeKind == TERM || nodeKind == FACTOR) {
        string type = check_type(node, root);
        if (!type.empty()) {
            node->line += " : " + type;
            node->type = type;
        }
    } else if (nodeKind == NUM) {
        node->line += " : int";
        node->type = "int";
    } else if (nodeKind == NIL) {
        node->line += " : int*";
        node->type = "int*";
    } else if (nodeKind == ID && symbolTable.find(node->tokens[1]) != symbolTable.end()) {
        node->line += " : " + symbolTable[node->tokens[1]];
        node->type = symbolTable[node->tokens[1]];
    }
    
    for (Node* child : node->children) {
        annotate_tree(child, root);
    }
}

void printTree(Node* node) {
    if (!node) {return;}
    cout << node->line << endl;
    for (Node* child : node->children) {
        printTree(child);
    }
}

Node* find_main(Node* root) {
    if (!root) return nullptr;
    
    if (root->tokens[0] == START && root->children.size() >= 2) {
        Node* procedures = root->children[1];
        if (procedures->tokens[0] == PROCEDURES && !procedures->children.empty()) {
            return procedures->children[0];
        }
    }
    
    return nullptr;
}

int main() {
    Node* root = parse_tree();
    if (!root) {
        format_error();
        return 0;
    }
    
    Node* wainNode = find_main(root);
    if (!wainNode || wainNode->tokens[0] != MAIN) {
        format_error(root);
    }
    
    process_decls(wainNode, root);
    check_return(wainNode, root);
    annotate_tree(root, root);
    printTree(root);

    delete root;
    return 0;
}