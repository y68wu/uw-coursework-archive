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
const string PLUS = "PLUS";
const string MINUS = "MINUS";
const string SLASH = "SLASH";
const string PCT = "PCT";
const string AMP = "AMP";
const string NEW = "NEW";
const string LBRACK = "LBRACK";
const string RBRACK = "RBRACK";

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

map<string, string> symbol_table;

vector<string> terminals = {
    "BOF", "INT", "WAIN", "LPAREN", "RPAREN", "COMMA",
    "LBRACE", "RBRACE", "RETURN", "SEMI", "STAR",
    "ID", "NUM", "NULL", "EOF", "BECOMES", "PLUS", 
    "MINUS", "SLASH", "PCT", "AMP", "NEW", "LBRACK", "RBRACK"
};

vector<string> tokenize(const string &s) {
    istringstream iss(s);
    vector<string> tokens;
    string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

Node* parse_tree() {
    string line;
    if (!getline(cin, line)) {
        return nullptr;
    }
    
    Node* node = new Node();
    node->line = line;
    node->tokens = tokenize(line);
    
    if (find(terminals.begin(), terminals.end(), node->tokens[0]) == terminals.end()) {
        if (node->tokens.size() == 2 && node->tokens[1] == ".EMPTY") {
            return node;
        }
        
        for (size_t i = 1; i < node->tokens.size(); i++) {
            Node* child = parse_tree();
            if (child) {
                node->children.push_back(child);
            }
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
    if (!typeNode || typeNode->tokens[0] != TYPE) {
        return "";
    }
    
    if (typeNode->children.size() == 1 && typeNode->children[0]->tokens[0] == INT) {
        return "int";
    } else if (typeNode->children.size() == 2 && 
               typeNode->children[0]->tokens[0] == INT && 
               typeNode->children[1]->tokens[0] == STAR) {
        return "int*";
    }
    
    return "";
}

pair<string, string> get_dcls(Node* dcl_node) {
    if (!dcl_node || dcl_node->tokens[0] != DCL || dcl_node->children.size() < 2) {
        return {"", ""};
    }
    
    string type = get_node_type(dcl_node->children[0]);
    string name = "";
    
    if (dcl_node->children[1]->tokens[0] == ID) {
        name = dcl_node->children[1]->tokens[1];
    }
    
    return {type, name};
}

void process_decls(Node* wain_node, Node* root) {
    symbol_table.clear();
    
    if (!wain_node || wain_node->tokens[0] != MAIN || wain_node->children.size() < 12) {
        format_error(root);
    }
    
    Node* param1 = wain_node->children[3];
    Node* param2 = wain_node->children[5];
    
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
    
    symbol_table[param1Name] = param1Type;
    symbol_table[param2Name] = param2Type;
    
    Node* dcls_node = wain_node->children[8];

    while (dcls_node && dcls_node->tokens[0] == DCLS && !dcls_node->children.empty()) {
        if (dcls_node->children.size() >= 5) {
            Node* dcl_node = dcls_node->children[1];
            Node* initValueNode = dcls_node->children[3];
            
            auto [varType, varName] = get_dcls(dcl_node);
            
            if (varName.empty()) {
                format_error(root);
            }
            if (symbol_table.find(varName) != symbol_table.end()) {
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
            
            symbol_table[varName] = varType;
            
            dcls_node = dcls_node->children[0];
        } else {
            break;
        }
    }
}

string check_lvalue_type(Node* node, Node* root);

string check_type(Node* node, Node* root) {
    if (!node) {
        return "";
    }
    
    string nodeKind = node->tokens[0];
    

    if (nodeKind == NUM) {
        return "int";
    } else if (nodeKind == NIL) {
        return "int*";
    } else if (nodeKind == ID) {
        string varName = node->tokens[1];
        if (symbol_table.find(varName) == symbol_table.end()) {
            format_error(root);
        }
        return symbol_table[varName];
    }
    

    if (nodeKind == EXPR) {
        if (node->children.size() == 1) {
            return check_type(node->children[0], root);
        } else if (node->children.size() == 3) {
            string leftType = check_type(node->children[0], root);
            string rightType = check_type(node->children[2], root);
            string op = node->children[1]->tokens[0];
            
            if (op == PLUS) {
                if (leftType == "int" && rightType == "int") {
                    return "int";
                }
                else if (leftType == "int*" && rightType == "int") {
                    return "int*";
                }
                else if (leftType == "int" && rightType == "int*") {
                    return "int*";
                }
            } else if (op == MINUS) {
                if (leftType == "int" && rightType == "int") {
                    return "int";
                }
                else if (leftType == "int*" && rightType == "int") {
                    return "int*";
                }
                else if (leftType == "int*" && rightType == "int*") {
                    return "int";
                }
            }
            
            format_error(root);
        }
    } else if (nodeKind == TERM) {
        if (node->children.size() == 1) {
            // term  -> factor
            return check_type(node->children[0], root);
        } else if (node->children.size() == 3) {
            // term -> term * factor, term -> term / factor, term - term % factor
            string leftType = check_type(node->children[0], root);
            string rightType = check_type(node->children[2], root);
            
            // *, / % int
            if (leftType == "int" && rightType == "int") {
                return "int";
            } else {
                format_error(root);
            }
        }
    } else if (nodeKind == FACTOR) {
        if (node->children.size() == 1) {
            // factor -> ID | NUM | NULL
            return check_type(node->children[0], root);
        } else if (node->children.size() == 2) {
            // factor -> AMP lvalue 
            if (node->children[0]->tokens[0] == AMP) {
                string lvalueType = check_lvalue_type(node->children[1], root);
                // int only
                if (lvalueType == "int") {
                    return "int*";
                } else {
                    format_error(root);
                }
            }
            // factor -> STAR factor 
            else if (node->children[0]->tokens[0] == STAR) {
                string factorType = check_type(node->children[1], root);
                if (factorType == "int*") {
                    return "int";
                } else {
                    format_error(root);
                }
            }
        } else if (node->children.size() == 3 && 
                   node->children[0]->tokens[0] == LPAREN && 
                   node->children[2]->tokens[0] == RPAREN) {
            // factor ->  LPAREN expr RPAREN
            return check_type(node->children[1], root);
        } else if (node->children.size() == 5 && 
                   node->children[0]->tokens[0] == NEW && 
                   node->children[1]->tokens[0] == INT) {
            // factor -> NEW INT LBRACK expr RBRACK
            string exprType = check_type(node->children[3], root);
            // array 
            if (exprType == "int") {
                return "int*";
            } else {
                format_error(root);
            }
        }
    } else if (nodeKind == LVALUE) {
        return check_lvalue_type(node, root);
    }
    
    format_error(root);
    return "";
}

string check_lvalue_type(Node* node, Node* root) {
    if (!node || node->tokens[0] != LVALUE) {
        format_error(root);
        return "";
    }
    
    if (node->children.size() == 1) {
        // lvalue → ID
        if (node->children[0]->tokens[0] == ID) {
            string varName = node->children[0]->tokens[1];
            if (symbol_table.find(varName) == symbol_table.end()) {
                format_error(root);
            }
            return symbol_table[varName];
        }
    } else if (node->children.size() == 2) {
        // lvalue -- STAR factor
        if (node->children[0]->tokens[0] == STAR) {
            string factorType = check_type(node->children[1], root);
            if (factorType == "int*") {
                return "int";
            } else {
                format_error(root);
            }
        }
    } else if (node->children.size() == 3) {
        // lvalue -> LPAREN lvalue RPAREN
        if (node->children[0]->tokens[0] == LPAREN && 
            node->children[2]->tokens[0] == RPAREN) {
            return check_lvalue_type(node->children[1], root);
        }
    }
    
    format_error(root);
    return "";
}

void check_return(Node* wain_node, Node* root) {
    if (wain_node->children.size() < 12) {
        format_error(root);
    }
    Node* returnExpr = nullptr;
    for (size_t i = 0; i < wain_node->children.size(); i++) {
        if (wain_node->children[i]->tokens[0] == RETURN && i + 1 < wain_node->children.size()) {
            returnExpr = wain_node->children[i + 1];
            break;
        }
    }
    
    if (!returnExpr) {
        format_error(root);
    }
    
    string return_type = check_type(returnExpr, root);
    if (return_type != "int") {
        format_error(root);
    }
}

void annotate_tree(Node* node, Node* root) {
    if (!node) {
        return;
    }
    
    string nodeKind = node->tokens[0];
    if (nodeKind == EXPR || nodeKind == TERM || nodeKind == FACTOR || nodeKind == LVALUE) {
        string type;
        if (nodeKind == LVALUE) {
            type = check_lvalue_type(node, root);
        } else {
            type = check_type(node, root);
        }
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
    } else if (nodeKind == ID && symbol_table.find(node->tokens[1]) != symbol_table.end()) {
        node->line += " : " + symbol_table[node->tokens[1]];
        node->type = symbol_table[node->tokens[1]];
    }
    
    for (Node* child : node->children) {
        annotate_tree(child, root);
    }
}

void print_tree(Node* node) {
    if (!node) {
        return;
    }
    cout << node->line << endl;
    for (Node* child : node->children) {
        print_tree(child);
    }
}

Node* find_main(Node* root) {
    if (!root) {
        return nullptr;
    }
    
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
    
    Node* wain_node = find_main(root);
    if (!wain_node || wain_node->tokens[0] != MAIN) {
        format_error(root);
    }
    
    process_decls(wain_node, root);
    check_return(wain_node, root);
    annotate_tree(root, root);
    print_tree(root);

    delete root;
    return 0;
}