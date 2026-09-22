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
const string PROCEDURE = "procedure";
const string TYPE = "type";
const string PLUS = "PLUS";
const string MINUS = "MINUS";
const string SLASH = "SLASH";
const string PCT = "PCT";
const string AMP = "AMP";
const string NEW = "NEW";
const string LBRACK = "LBRACK";
const string RBRACK = "RBRACK";
const string PARAMS = "params";
const string PARAMLIST = "paramlist";
const string ARGLIST = "arglist";
const string GETCHAR = "GETCHAR";


struct process_sig {
    vector<string> param_types;
    bool is_declared;  
};

struct Node {
    string line;
    vector<string> tokens;
    vector<Node*> children;
    string type;
    bool isProcName;

    Node() : isProcName(false) {}
    
    ~Node() {
        for (Node* child : children) {
            delete child;
        }
        children.clear();
    }
};

map<string, process_sig> process_table;
map<string, map<string, string>> symbol_tables;
string cur_process;

vector<string> proc_order;

vector<string> terminals = {
    "BOF", "INT", "WAIN", "LPAREN", "RPAREN", "COMMA",
    "LBRACE", "RBRACE", "RETURN", "SEMI", "STAR",
    "ID", "NUM", "NULL", "EOF", "BECOMES", "PLUS", 
    "MINUS", "SLASH", "PCT", "AMP", "NEW", "LBRACK", "RBRACK",
    "GETCHAR"
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
        if (node->tokens.size() == 2 && node->tokens[1] == EMPTY) {
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

string get_node_type(Node* type_node) {
    if (!type_node || type_node->tokens[0] != TYPE) {
        return "";
    }
    if (type_node->children.size() == 1 && type_node->children[0]->tokens[0] == INT) {
        return "int";
    } else if (type_node->children.size() == 2 && 
               type_node->children[0]->tokens[0] == INT && 
               type_node->children[1]->tokens[0] == STAR) {
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

string check_type(Node* node, Node* root);
string check_lvalue_type(Node* node, Node* root);

void collect_procedures(Node* root) {
    if (!root || root->tokens[0] != START) {
        format_error(root);
    }
    Node* process_node = root->children[1];
    if (!process_node || process_node->tokens[0] != PROCEDURES) {
        format_error(root);
    }
    process_table.clear();
    symbol_tables.clear();
    proc_order.clear();
    
    Node* cur_node = process_node;
    while (cur_node && cur_node->tokens[0] == PROCEDURES) {
        if (cur_node->children.size() >= 2 && 
            cur_node->children[0]->tokens[0] == PROCEDURE) {
            Node* proce_node = cur_node->children[0];
            if (proce_node->children.size() < 2 || proce_node->children[1]->tokens[0] != ID) {
                format_error(root);
            }

            Node* procNameNode = proce_node->children[1];
            procNameNode->isProcName = true;
            string proce_name = procNameNode->tokens[1];
            proc_order.push_back(proce_name);
            if (process_table.find(proce_name) != process_table.end()) {
                format_error(root);
            }
            process_table[proce_name] = process_sig{vector<string>(), false};
            cur_node = cur_node->children[1];
        } else if (cur_node->children.size() == 1 && cur_node->children[0]->tokens[0] == MAIN) {
            proc_order.push_back("wain");
            process_table["wain"] = process_sig{vector<string>(), false};
            break;
        } else {
            format_error(root);
        }
    }
    
    cur_node = process_node;
    while (cur_node && cur_node->tokens[0] == PROCEDURES) {
        if (cur_node->children.size() >= 2 && cur_node->children[0]->tokens[0] == PROCEDURE) {
            Node* proce_node = cur_node->children[0];
            string proce_name = proce_node->children[1]->tokens[1];
            if (proce_node->children.size() < 5 || proce_node->children[3]->tokens[0] != PARAMS) {
                format_error(root);
            }
            Node* params_node = proce_node->children[3];
            vector<string> param_types;
            if (params_node->children.size() == 1 && 
                params_node->children[0]->tokens[0] == PARAMLIST) {
                Node* param_list_node = params_node->children[0];
                while (param_list_node && param_list_node->tokens[0] == PARAMLIST) {
                    if (param_list_node->children.size() >= 1 && 
                        param_list_node->children[0]->tokens[0] == DCL) {
                        auto [paramType, paramName] = get_dcls(param_list_node->children[0]);
                        param_types.push_back(paramType);
                        if (param_list_node->children.size() >= 3 && 
                            param_list_node->children[2]->tokens[0] == PARAMLIST) {
                            param_list_node = param_list_node->children[2];
                        } else {
                            break;
                        }
                    } else {
                        break;
                    }
                }
            }
            process_table[proce_name].param_types = param_types;
            process_table[proce_name].is_declared = true;
            symbol_tables[proce_name] = map<string, string>();
            cur_node = cur_node->children[1];
        } else if (cur_node->children.size() == 1 && 
                   cur_node->children[0]->tokens[0] == MAIN) {
            Node* wainNode = cur_node->children[0];
            if (wainNode->children.size() < 6) {
                format_error(root);
            }
            auto [param1Type, param1Name] = get_dcls(wainNode->children[3]);
            auto [param2Type, param2Name] = get_dcls(wainNode->children[5]);
            if (param1Name.empty() || param2Name.empty() || param2Type != "int") {
                format_error(root);
            }
            process_table["wain"].param_types = {param1Type, param2Type};
            process_table["wain"].is_declared = true;
            symbol_tables["wain"] = map<string, string>();
            break;
        } else {
            format_error(root);
        }
    }
}

void process_decls(Node* proce_node, string proce_name, Node* root) {
    cur_process = proce_name;
    auto& symTable = symbol_tables[proce_name];
    symTable.clear();

    if (proce_name == "wain") {
        if (proce_node->children.size() < 12) {
            format_error(root);
        }
        Node* param1 = proce_node->children[3];
        Node* param2 = proce_node->children[5];
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

        symTable[param1Name] = param1Type;
        symTable[param2Name] = param2Type;
        Node* dcls_node = proce_node->children[8];
        
        while (dcls_node && dcls_node->tokens[0] == DCLS && !dcls_node->children.empty()) {
            if (dcls_node->children.size() >= 5) {
                Node* dcl_node = dcls_node->children[1];
                Node* initValueNode = dcls_node->children[3];
                
                auto [varType, var_name] = get_dcls(dcl_node);
                if (var_name.empty()) {
                    format_error(root);
                }
                if (symTable.find(var_name) != symTable.end()) {
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
                
                symTable[var_name] = varType;
                dcls_node = dcls_node->children[0];
            } else {
                break;
            }
        }
    }
    else {
        if (proce_node->children.size() < 9) {
            format_error(root);
        }
        
        Node* params_node = proce_node->children[3];
        
        if (params_node->children.size() == 1 && 
            params_node->children[0]->tokens[0] == PARAMLIST) {
            Node* param_list_node = params_node->children[0];
            vector<pair<string, string>> params;
            while (param_list_node && param_list_node->tokens[0] == PARAMLIST) {
                if (param_list_node->children.size() >= 1 && 
                    param_list_node->children[0]->tokens[0] == DCL) {
                    auto [paramType, paramName] = get_dcls(param_list_node->children[0]);
                    if (paramName.empty()) {
                        format_error(root);
                    }
                    for (const auto& [prevName, prevType] : params) {
                        if (paramName == prevName) {
                            format_error(root);
                        }
                    }
                    params.push_back({paramName, paramType});
                    if (param_list_node->children.size() >= 3 && 
                        param_list_node->children[2]->tokens[0] == PARAMLIST) {
                        param_list_node = param_list_node->children[2];
                    } else {
                        break;
                    }
                } else {
                    break;
                }
            }
            for (const auto& [name, type] : params) {
                symTable[name] = type;
            }
        }
        
        int dclsIndex = -1;
        for (int i = 0; i < proce_node->children.size(); i++) {
            if (proce_node->children[i]->tokens[0] == DCLS) {
                dclsIndex = i;
                break;
            }
        }
        if (dclsIndex == -1) {
            format_error(root);
        }
        Node* dcls_node = proce_node->children[dclsIndex];
        while (dcls_node && dcls_node->tokens[0] == DCLS && !dcls_node->children.empty()) {
            if (dcls_node->children.size() >= 5) {
                Node* dcl_node = dcls_node->children[1];
                Node* initValueNode = dcls_node->children[3];
                auto [varType, var_name] = get_dcls(dcl_node);
                if (var_name.empty()) {
                    format_error(root);
                }
                if (symTable.find(var_name) != symTable.end()) {
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
                symTable[var_name] = varType;
                dcls_node = dcls_node->children[0];
            } else {
                break;
            }
        }
    }
}

bool check_proc_call(const string& proce_name, const vector<string>& argTypes, Node* root) {
    if (process_table.find(proce_name) == process_table.end()) {
        format_error(root);
        return false;
    }
    if (!process_table[proce_name].is_declared) {
        format_error(root);
        return false;
    }
    if (process_table[proce_name].param_types.size() != argTypes.size()) {
        format_error(root);
        return false;
    }
    if (cur_process != proce_name) {
        auto itCaller = find(proc_order.begin(), proc_order.end(), cur_process);
        auto itCallee = find(proc_order.begin(), proc_order.end(), proce_name);
        if (itCaller == proc_order.end() || itCallee == proc_order.end() ||
            distance(proc_order.begin(), itCallee) >= distance(proc_order.begin(), itCaller)) {
            format_error(root);
            return false;
        }
    }
    for (size_t i = 0; i < argTypes.size(); i++) {
        if (process_table[proce_name].param_types[i] != argTypes[i]) {
            format_error(root);
            return false;
        }
    }
    return true;
}

vector<string> process_args(Node* arglistNode, Node* root) {
    vector<string> argTypes;
    if (!arglistNode) {
        return argTypes;
    }
    if (arglistNode->tokens[0] != ARGLIST) {
        format_error(root);
    }
    Node* cur_node = arglistNode;
    while (cur_node && cur_node->tokens[0] == ARGLIST) {
        if (cur_node->children.size() >= 1) {
            string argType = check_type(cur_node->children[0], root);
            argTypes.push_back(argType);
            if (cur_node->children.size() >= 3 && 
                cur_node->children[2]->tokens[0] == ARGLIST) {
                cur_node = cur_node->children[2];
            } else {
                break;
            }
        } else {
            break;
        }
    }
    return argTypes;
}

string check_lvalue_type(Node* node, Node* root) {
    if (!node || node->tokens[0] != LVALUE) {
        format_error(root);
        return "";
    }
    if (node->children.size() == 1) {
        if (node->children[0]->tokens[0] == ID) {
            string var_name = node->children[0]->tokens[1];
            auto& symTable = symbol_tables[cur_process];
            if (symTable.find(var_name) == symTable.end()) {
                format_error(root);
            }
            return symTable[var_name];
        }
    } else if (node->children.size() == 2) {
        if (node->children[0]->tokens[0] == STAR) {
            string factorType = check_type(node->children[1], root);
            if (factorType == "int*") {
                return "int";
            } else {
                format_error(root);
            }
        }
    } else if (node->children.size() == 3) {
        if (node->children[0]->tokens[0] == LPAREN && 
            node->children[2]->tokens[0] == RPAREN) {
            return check_lvalue_type(node->children[1], root);
        }
    }
    format_error(root);
    return "";
}

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
        string name = node->tokens[1];
        auto& symTable = symbol_tables[cur_process];
        if (symTable.find(name) != symTable.end()) {
            return symTable[name];
        }
        format_error(root);
        return "";
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
            return check_type(node->children[0], root);
        } else if (node->children.size() == 3) {
            string leftType = check_type(node->children[0], root);
            string rightType = check_type(node->children[2], root);
            if (leftType == "int" && rightType == "int") {
                return "int";
            } else {
                format_error(root);
            }
        }
    } else if (nodeKind == FACTOR) {
        if (node->children.size() == 1) {
            return check_type(node->children[0], root);
        } else if (node->children.size() == 2) {
            if (node->children[0]->tokens[0] == AMP) {
                string lvalueType = check_lvalue_type(node->children[1], root);
                if (lvalueType == "int") {
                    return "int*";
                } else {
                    format_error(root);
                }
            }
            else if (node->children[0]->tokens[0] == STAR) {
                string factorType = check_type(node->children[1], root);
                if (factorType == "int*") {
                    return "int";
                } else {
                    format_error(root);
                }
            }
        } else if (node->children.size() == 3) {
            if (node->children[0]->tokens[0] == LPAREN && 
                node->children[2]->tokens[0] == RPAREN) {
                return check_type(node->children[1], root);
            }
            else if (node->children[0]->tokens[0] == ID && 
                     node->children[1]->tokens[0] == LPAREN &&
                     node->children[2]->tokens[0] == RPAREN) {
                string proce_name = node->children[0]->tokens[1];
                auto& symTable = symbol_tables[cur_process];
                if (symTable.find(proce_name) != symTable.end()) {
                    format_error(root);
                }
                vector<string> emptyArgs;
                if (check_proc_call(proce_name, emptyArgs, root)) {
                    return "int";
                }
            }
            else if (node->children[0]->tokens[0] == GETCHAR && 
                     node->children[1]->tokens[0] == LPAREN &&
                     node->children[2]->tokens[0] == RPAREN) {
                return "int";
            }
        } else if (node->children.size() == 4) {
            if (node->children[0]->tokens[0] == ID && 
                node->children[1]->tokens[0] == LPAREN &&
                node->children[2]->tokens[0] == ARGLIST &&
                node->children[3]->tokens[0] == RPAREN) {
                string proce_name = node->children[0]->tokens[1];
                auto& symTable = symbol_tables[cur_process];
                if (symTable.find(proce_name) != symTable.end()) {
                    format_error(root);
                }
                vector<string> argTypes = process_args(node->children[2], root);
                if (check_proc_call(proce_name, argTypes, root)) {
                    return "int";
                }
            }
        } else if (node->children.size() == 5 && 
                   node->children[0]->tokens[0] == NEW && 
                   node->children[1]->tokens[0] == INT) {
            string exprType = check_type(node->children[3], root);
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

void check_returns(Node* root) {
    if (!root || root->tokens[0] != START) {
        format_error(root);
    }
    Node* process_node = root->children[1];
    if (!process_node || process_node->tokens[0] != PROCEDURES) {
        format_error(root);
    }
    Node* cur_node = process_node;
    while (cur_node && cur_node->tokens[0] == PROCEDURES) {
        if (cur_node->children.size() >= 2 && 
            cur_node->children[0]->tokens[0] == PROCEDURE) {
            Node* proce_node = cur_node->children[0];
            string proce_name = proce_node->children[1]->tokens[1];
            cur_process = proce_name;
            int returnIndex = -1;
            for (int i = 0; i < proce_node->children.size(); i++) {
                if (proce_node->children[i]->tokens[0] == RETURN) {
                    returnIndex = i;
                    break;
                }
            }
            if (returnIndex == -1 || returnIndex + 1 >= proce_node->children.size()) {
                format_error(root);
            }
            Node* returnExpr = proce_node->children[returnIndex + 1];
            string returnType = check_type(returnExpr, root);
            if (returnType != "int") {
                format_error(root);
            }
            cur_node = cur_node->children[1];
        } else if (cur_node->children.size() == 1 && 
                   cur_node->children[0]->tokens[0] == MAIN) {
            Node* wainNode = cur_node->children[0];
            cur_process = "wain";
            int returnIndex = -1;
            for (int i = 0; i < wainNode->children.size(); i++) {
                if (wainNode->children[i]->tokens[0] == RETURN) {
                    returnIndex = i;
                    break;
                }
            }
            if (returnIndex == -1 || returnIndex + 1 >= wainNode->children.size()) {
                format_error(root);
            }
            Node* returnExpr = wainNode->children[returnIndex + 1];
            string returnType = check_type(returnExpr, root);
            if (returnType != "int") {
                format_error(root);
            }
            break;
        } else {
            format_error(root);
        }
    }
}

void process_all_procedures(Node* root) {
    if (!root || root->tokens[0] != START) {
        format_error(root);
    }
    Node* process_node = root->children[1];
    if (!process_node || process_node->tokens[0] != PROCEDURES) {
        format_error(root);
    }
    Node* cur_node = process_node;
    while (cur_node && cur_node->tokens[0] == PROCEDURES) {
        if (cur_node->children.size() >= 2 && 
            cur_node->children[0]->tokens[0] == PROCEDURE) {
            Node* proce_node = cur_node->children[0];
            string proce_name = proce_node->children[1]->tokens[1];
            process_decls(proce_node, proce_name, root);
            cur_node = cur_node->children[1];
        } else if (cur_node->children.size() == 1 && 
                   cur_node->children[0]->tokens[0] == MAIN) {
            Node* wainNode = cur_node->children[0];
            process_decls(wainNode, "wain", root);
            break;
        } else {
            format_error(root);
        }
    }
}

void annotate_tree(Node* node, Node* root, const string &currentProc) {
    if (!node)
        return;
    cur_process = currentProc;
    
    string currProc = currentProc;
    if (node->tokens[0] == MAIN) {
        currProc = "wain";
    } else if (node->tokens[0] == PROCEDURE) {
        if (node->children.size() >= 2 && node->children[1]->tokens[0] == ID) {
            currProc = node->children[1]->tokens[1];
        }
    }
    cur_process = currProc;
    
    if (node->tokens[0] == EXPR || node->tokens[0] == TERM ||
        node->tokens[0] == FACTOR || node->tokens[0] == LVALUE) {
        string t;
        if (node->tokens[0] == LVALUE)
            t = check_lvalue_type(node, root);
        else
            t = check_type(node, root);
        if (!t.empty()) {
            node->line += " : " + t;
            node->type = t;
        }
    }
    else if (node->tokens[0] == NUM) {
        node->line += " : int";
        node->type = "int";
    } else if (node->tokens[0] == NIL) {
        node->line += " : int*";
        node->type = "int*";
    } else if (node->tokens[0] == ID) {
        if (!node->isProcName) {
            if (symbol_tables.find(currProc) != symbol_tables.end() &&
                symbol_tables[currProc].find(node->tokens[1]) != symbol_tables[currProc].end()) {
                string varType = symbol_tables[currProc][node->tokens[1]];
                node->line += " : " + varType;
                node->type = varType;
            }
        }
    }
    
    for (Node* child : node->children) {
        annotate_tree(child, root, currProc);
    }
}

void print_tree(Node* node) {
    if (!node)
        return;
    cout << node->line << endl;
    for (Node* child : node->children)
        print_tree(child);
}

Node* find_main(Node* root) {
    if (!root)
        return nullptr;
    if (root->tokens[0] == START && root->children.size() >= 2) {
        Node* procedures = root->children[1];
        if (procedures->tokens[0] == PROCEDURES) {
            Node* cur_node = procedures;
            while (cur_node && cur_node->tokens[0] == PROCEDURES) {
                if (cur_node->children.size() == 1 && 
                    cur_node->children[0]->tokens[0] == MAIN) {
                    return cur_node->children[0];
                }
                if (cur_node->children.size() >= 2) {
                    cur_node = cur_node->children[1];
                } else {
                    break;
                }
            }
        }
    }
    return nullptr;
}
void deleteNode(Node* node) {
    if (node == nullptr) return;
    
    // Recursively delete all children first
    for (Node* child : node->children) {
        deleteNode(child);
    }
    
    // Clear the children vector to prevent any potential double-deletion
    node->children.clear();
    
    // Delete the node itself
    delete node;
}
int main() {
    Node* root = parse_tree();
    if (!root) {
        format_error();
        return 0;
    }
    
    try {
        collect_procedures(root);
        process_all_procedures(root);
        check_returns(root);
        annotate_tree(root, root, "");
        print_tree(root);
    } catch (...) {
        // Ensure memory is freed even if an exception occurs
        deleteNode(root);
        throw;
    }
    
    // Use the new deletion method
    deleteNode(root);
    return 0;
}