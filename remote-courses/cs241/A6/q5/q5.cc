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

// for stateme
const string STATEMENT = "statement";
const string STATEMENTS = "statements";
const string IF = "IF";
const string ELSE = "ELSE";
const string WHILE = "WHILE";
const string PRINTLN = "PRINTLN";
const string PUTCHAR = "PUTCHAR";
const string DELETE = "DELETE";
const string LBRACE = "LBRACE";
const string RBRACE = "RBRACE";
const string TEST = "test";
const string EQ = "EQ";
const string NE = "NE";
const string LT = "LT";
const string GT = "GT";
const string LE = "LE";
const string GE = "GE";

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

bool has_error = false;

map<string, process_sig> process_table;
map<string, map<string, string>> symbol_tables;
string cur_process;

vector<string> proc_order;

vector<string> terminals = {
    "BOF", "INT", "WAIN", "LPAREN", "RPAREN", "COMMA",
    "LBRACE", "RBRACE", "RETURN", "SEMI", "STAR",
    "ID", "NUM", "NULL", "EOF", "BECOMES", "PLUS", 
    "MINUS", "SLASH", "PCT", "AMP", "NEW", "LBRACK", "RBRACK",
    "GETCHAR", "IF", "ELSE", "WHILE", "PRINTLN", "PUTCHAR", 
    "DELETE", "EQ", "NE", "LT", "GT", "LE", "GE"
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
    has_error = true;
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
void check_statements(Node* node, Node* root);

void collect_procedures(Node* root) {
    if (!root || root->tokens[0] != START) {
        format_error(root);
        return;
    }
    Node* process_node = root->children[1];
    if (!process_node || process_node->tokens[0] != PROCEDURES) {
        format_error(root);
        return;
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
                return;
            }

            Node*proc_name_node  = proce_node->children[1];
            proc_name_node->isProcName = true;
            string proce_name = proc_name_node->tokens[1];
            proc_order.push_back(proce_name);
            if (process_table.find(proce_name) != process_table.end()) {
                format_error(root);
                return;
            }
            process_table[proce_name] = process_sig{vector<string>(), false};
            cur_node = cur_node->children[1];
        } else if (cur_node->children.size() == 1 && cur_node->children[0]->tokens[0] == MAIN) {
            proc_order.push_back("wain");
            process_table["wain"] = process_sig{vector<string>(), false};
            break;
        } else {
            format_error(root);
            return;
        }
    }
    cur_node = process_node;
    while (cur_node && cur_node->tokens[0] == PROCEDURES) {
        if (cur_node->children.size() >= 2 && cur_node->children[0]->tokens[0] == PROCEDURE) {
            Node* proce_node = cur_node->children[0];
            string proce_name = proce_node->children[1]->tokens[1];
            if (proce_node->children.size() < 5 || proce_node->children[3]->tokens[0] != PARAMS) {
                format_error(root);
                return;
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
            Node* wain_node = cur_node->children[0];
            if (wain_node->children.size() < 6) {
                format_error(root);
                return;
            }
            auto [param1Type, param1Name] = get_dcls(wain_node->children[3]);
            auto [param2Type, param2Name] = get_dcls(wain_node->children[5]);
            if (param1Name.empty() || param2Name.empty() || param2Type != "int") {
                format_error(root);
                return;
            }
            process_table["wain"].param_types = {param1Type, param2Type};
            process_table["wain"].is_declared = true;
            symbol_tables["wain"] = map<string, string>();
            break;
        } else {
            format_error(root);
            return;
        }
    }
}

void process_decls(Node* proce_node, string proce_name, Node* root) {
    if (has_error) return;
    
    cur_process = proce_name;
    auto& symTable = symbol_tables[proce_name];
    symTable.clear();
    if (proce_name == "wain") {
        if (proce_node->children.size() < 12) {
            format_error(root);
            return;
        }
        Node* param1 = proce_node->children[3];
        Node* param2 = proce_node->children[5];
        auto [param1Type, param1Name] = get_dcls(param1);
        auto [param2Type, param2Name] = get_dcls(param2);
        
        if (param1Name.empty() || param2Name.empty()) {
            format_error(root);
            return;
        }
        if (param1Name == param2Name) {
            format_error(root);
            return;
        }
        if (param2Type != "int") {
            format_error(root);
            return;
        }

        symTable[param1Name] = param1Type;
        symTable[param2Name] = param2Type;
        Node* dcls_node = proce_node->children[8];
        
        while (dcls_node && dcls_node->tokens[0] == DCLS && !dcls_node->children.empty()) {
            if (dcls_node->children.size() >= 5) {
                Node* dcl_node = dcls_node->children[1];
                Node* initValueNode = dcls_node->children[3];
                
                auto [var_type, var_name] = get_dcls(dcl_node);
                if (var_name.empty()) {
                    format_error(root);
                    return;
                }
                if (symTable.find(var_name) != symTable.end()) {
                    format_error(root);
                    return;
                }
    
                string initType = "";
                if (initValueNode->tokens[0] == NUM) {
                    initType = "int";
                } else if (initValueNode->tokens[0] == NIL) {
                    initType = "int*";
                } else {
                    format_error(root);
                    return;
                }
    
                if (var_type != initType) {
                    format_error(root);
                    return;
                }
                
                symTable[var_name] = var_type;
                dcls_node = dcls_node->children[0];
            } else {
                break;
            }
        }
        
        Node* stmts_node = proce_node->children[9];
        check_statements(stmts_node, root);
    }
    else {
        if (proce_node->children.size() < 9) {
            format_error(root);
            return;
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
                        return;
                    }
                    for (const auto& [prevName, prevType] : params) {
                        if (paramName == prevName) {
                            format_error(root);
                            return;
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
        int statIndex = -1;
        
        for (int i = 0; i < proce_node->children.size(); i++) {
            if (proce_node->children[i]->tokens[0] == DCLS) {
                dclsIndex = i;
            }
            if (proce_node->children[i]->tokens[0] == STATEMENTS) {
                statIndex = i;
            }
        }
        
        if (dclsIndex == -1 || statIndex == -1) {
            format_error(root);
            return;
        }
        
        Node* dcls_node = proce_node->children[dclsIndex];
        while (dcls_node && dcls_node->tokens[0] == DCLS && !dcls_node->children.empty()) {
            if (dcls_node->children.size() >= 5) {
                Node* dcl_node = dcls_node->children[1];
                Node* initValueNode = dcls_node->children[3];
                auto [var_type, var_name] = get_dcls(dcl_node);
                if (var_name.empty()) {
                    format_error(root);
                    return;
                }
                if (symTable.find(var_name) != symTable.end()) {
                    format_error(root);
                    return;
                }
                string initType = "";
                if (initValueNode->tokens[0] == NUM) {
                    initType = "int";
                } else if (initValueNode->tokens[0] == NIL) {
                    initType = "int*";
                } else {
                    format_error(root);
                    return;
                }
                if (var_type != initType) {
                    format_error(root);
                    return;
                }
                symTable[var_name] = var_type;
                dcls_node = dcls_node->children[0];
            } else {
                break;
            }
        }
        
        Node* stmts_node = proce_node->children[statIndex];
        check_statements(stmts_node, root);
    }
}

bool check_proc_call(const string& proce_name, const vector<string>& argTypes, Node* root) {
    if (has_error) return false;
    
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
        return argTypes;
    }
    Node* cur_node = arglistNode;
    while (cur_node && cur_node->tokens[0] == ARGLIST) {
        if (cur_node->children.size() >= 1) {
            string argType = check_type(cur_node->children[0], root);
            if (has_error) return argTypes;
            
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
    if (has_error) return "";
    
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
                return "";
            }
            return symTable[var_name];
        }
    } else if (node->children.size() == 2) {
        if (node->children[0]->tokens[0] == STAR) {
            string factorType = check_type(node->children[1], root);
            if (has_error) return "";
            
            if (factorType == "int*") {
                return "int";
            } else {
                format_error(root);
                return "";
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
    if (has_error) return "";
    
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
            if (has_error) return "";
            
            string rightType = check_type(node->children[2], root);
            if (has_error) return "";
            
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
            return "";
        }
    } else if (nodeKind == TERM) {
        if (node->children.size() == 1) {
            return check_type(node->children[0], root);
        } else if (node->children.size() == 3) {
            string leftType = check_type(node->children[0], root);
            if (has_error) return "";
            
            string rightType = check_type(node->children[2], root);
            if (has_error) return "";
            
            if (leftType == "int" && rightType == "int") {
                return "int";
            } else {
                format_error(root);
                return "";
            }
        }
    } else if (nodeKind == FACTOR) {
        if (node->children.size() == 1) {
            return check_type(node->children[0], root);
        } else if (node->children.size() == 2) {
            if (node->children[0]->tokens[0] == AMP) {
                string lvalueType = check_lvalue_type(node->children[1], root);
                if (has_error) return "";
                
                if (lvalueType == "int") {
                    return "int*";
                } else {
                    format_error(root);
                    return "";
                }
            }
            else if (node->children[0]->tokens[0] == STAR) {
                string factorType = check_type(node->children[1], root);
                if (has_error) return "";
                
                if (factorType == "int*") {
                    return "int";
                } else {
                    format_error(root);
                    return "";
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
                    return "";
                }
                vector<string> emptyArgs;
                if (check_proc_call(proce_name, emptyArgs, root)) {
                    return "int";
                }
                return "";
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
                    return "";
                }
                vector<string> argTypes = process_args(node->children[2], root);
                if (has_error) return "";
                
                if (check_proc_call(proce_name, argTypes, root)) {
                    return "int";
                }
                return "";
            }
        } else if (node->children.size() == 5 && 
                   node->children[0]->tokens[0] == NEW && 
                   node->children[1]->tokens[0] == INT) {
            string exprType = check_type(node->children[3], root);
            if (has_error) return "";
            
            if (exprType == "int") {
                return "int*";
            } else {
                format_error(root);
                return "";
            }
        }
    } else if (nodeKind == LVALUE) {
        return check_lvalue_type(node, root);
    }
    format_error(root);
    return "";
}

void check_test(Node* node, Node* root) {
    if (has_error) return;
    
    if (!node || !node->children.size() >= 3 || node->tokens[0] != TEST) {
        format_error(root);
        return;
    }
    
    string leftType = check_type(node->children[0], root);
    if (has_error) return;
    string rightType = check_type(node->children[2], root);
    if (has_error) return;
    
    if (leftType != rightType) {
        format_error(root);
        return;
    }
    
    string testOp = node->children[1]->tokens[0];
    if (testOp != EQ && testOp != NE && testOp != LT && 
        testOp != GT && testOp != LE && testOp != GE) {
        format_error(root);
        return;
    }
}


void check_statement(Node* node, Node* root) {
    if (has_error || !node || node->tokens[0] != STATEMENT) {
        return;
    }
    
    if (node->children.size() >= 1) {
        if (node->children.size() == 4 && 
            node->children[0]->tokens[0] == LVALUE && 
            node->children[1]->tokens[0] == BECOMES &&
            node->children[2]->tokens[0] == EXPR &&
            node->children[3]->tokens[0] == SEMI) {
            
            string lvalueType = check_lvalue_type(node->children[0], root);
            if (has_error) return;
            
            string exprType = check_type(node->children[2], root);
            if (has_error) return;
            
            if (lvalueType != exprType) {
                format_error(root);
                return;
            }
        }

        else if (node->children.size() == 11 && 
                 node->children[0]->tokens[0] == IF && 
                 node->children[1]->tokens[0] == LPAREN &&
                 node->children[2]->tokens[0] == TEST &&
                 node->children[3]->tokens[0] == RPAREN &&
                 node->children[4]->tokens[0] == LBRACE &&
                 node->children[5]->tokens[0] == STATEMENTS &&
                 node->children[6]->tokens[0] == RBRACE &&
                 node->children[7]->tokens[0] == ELSE &&
                 node->children[8]->tokens[0] == LBRACE &&
                 node->children[9]->tokens[0] == STATEMENTS &&
                 node->children[10]->tokens[0] == RBRACE) {
            
            check_test(node->children[2], root);
            if (has_error) return;
            
            check_statements(node->children[5], root);
            if (has_error) return;
            
            check_statements(node->children[9], root);
            if (has_error) return;
        }

        else if (node->children.size() == 7 && 
                 node->children[0]->tokens[0] == WHILE && 
                 node->children[1]->tokens[0] == LPAREN &&
                 node->children[2]->tokens[0] == TEST &&
                 node->children[3]->tokens[0] == RPAREN &&
                 node->children[4]->tokens[0] == LBRACE &&
                 node->children[5]->tokens[0] == STATEMENTS &&
                 node->children[6]->tokens[0] == RBRACE) {
            
            check_test(node->children[2], root);
            if (has_error) return;
            
            check_statements(node->children[5], root);
            if (has_error) return;
        }
        else if (node->children.size() == 5 && 
                 node->children[0]->tokens[0] == PRINTLN && 
                 node->children[1]->tokens[0] == LPAREN &&
                 node->children[2]->tokens[0] == EXPR &&
                 node->children[3]->tokens[0] == RPAREN &&
                 node->children[4]->tokens[0] == SEMI) {
            
            string exprType = check_type(node->children[2], root);
            if (has_error) return;
            
            if (exprType != "int") {
                format_error(root);
                return;
            }
        }
        
        else if (node->children.size() == 5 && 
                 node->children[0]->tokens[0] == PUTCHAR && 
                 node->children[1]->tokens[0] == LPAREN &&
                 node->children[2]->tokens[0] == EXPR &&
                 node->children[3]->tokens[0] == RPAREN &&
                 node->children[4]->tokens[0] == SEMI) {
            
            string exprType = check_type(node->children[2], root);
            if (has_error) return;
            
            if (exprType != "int") {
                format_error(root);
                return;
            }
        }
        else if (node->children.size() == 5 && 
                 node->children[0]->tokens[0] == DELETE && 
                 node->children[1]->tokens[0] == LBRACK &&
                 node->children[2]->tokens[0] == RBRACK &&
                 node->children[3]->tokens[0] == EXPR &&
                 node->children[4]->tokens[0] == SEMI) {
            
            string exprType = check_type(node->children[3], root);
            if (has_error) return;
            
            if (exprType != "int*") {
                format_error(root);
                return;
            }
        }
        else {
            format_error(root);
            return;
        }
    }
}

void check_statements(Node* node, Node* root) {
    if (has_error || !node) return;
    
    if (node->tokens[0] == STATEMENTS) {
        if (node->children.empty() || node->children[0]->tokens[0] == EMPTY) {
            return;
        }
        
        if (node->children.size() == 2 && 
            node->children[0]->tokens[0] == STATEMENTS && 
            node->children[1]->tokens[0] == STATEMENT) {
            
            check_statements(node->children[0], root);
            if (has_error) return;
            check_statement(node->children[1], root);
            if (has_error) return;
        }
        else {
            format_error(root);
            return;
        }
    }
}


void check_returns(Node* root) {
    if (has_error) return;
    
    if (!root || root->tokens[0] != START) {
        format_error(root);
        return;
    }
    Node* process_node = root->children[1];
    if (!process_node || process_node->tokens[0] != PROCEDURES) {
        format_error(root);
        return;
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
                return;
            }
            Node* returnExpr = proce_node->children[returnIndex + 1];
            string returnType = check_type(returnExpr, root);
            if (has_error) return;
            
            if (returnType != "int") {
                format_error(root);
                return;
            }
            cur_node = cur_node->children[1];
        } else if (cur_node->children.size() == 1 && 
                   cur_node->children[0]->tokens[0] == MAIN) {
            Node* wain_node = cur_node->children[0];
            cur_process = "wain";
            int returnIndex = -1;
            for (int i = 0; i < wain_node->children.size(); i++) {
                if (wain_node->children[i]->tokens[0] == RETURN) {
                    returnIndex = i;
                    break;
                }
            }
            if (returnIndex == -1 || returnIndex + 1 >= wain_node->children.size()) {
                format_error(root);
                return;
            }
            Node* returnExpr = wain_node->children[returnIndex + 1];
            string returnType = check_type(returnExpr, root);
            if (has_error) return;
            
            if (returnType != "int") {
                format_error(root);
                return;
            }
            
            int statIndex = -1;
            for (int i = 0; i < wain_node->children.size(); i++) {
                if (wain_node->children[i]->tokens[0] == STATEMENTS) {
                    statIndex = i;
                    break;
                }
            }
            
            if (statIndex != -1) {
                check_statements(wain_node->children[statIndex], root);
                if (has_error) return;
            }
            break;
        } else {
            format_error(root);
            return;
        }
    }
}

void process_all_procedures(Node* root) {
    if (has_error) return;
    
    if (!root || root->tokens[0] != START) {
        format_error(root);
        return;
    }
    Node* process_node = root->children[1];
    if (!process_node || process_node->tokens[0] != PROCEDURES) {
        format_error(root);
        return;
    }
    Node* cur_node = process_node;
    while (cur_node && cur_node->tokens[0] == PROCEDURES) {
        if (cur_node->children.size() >= 2 && 
            cur_node->children[0]->tokens[0] == PROCEDURE) {
            Node* proce_node = cur_node->children[0];
            string proce_name = proce_node->children[1]->tokens[1];
            process_decls(proce_node, proce_name, root);
            if (has_error) return;
            
            cur_node = cur_node->children[1];
        } else if (cur_node->children.size() == 1 && 
                   cur_node->children[0]->tokens[0] == MAIN) {
            Node* wain_node = cur_node->children[0];
            process_decls(wain_node, "wain", root);
            if (has_error) return;
            break;
        } else {
            format_error(root);
            return;
        }
    }
}

void annotate_tree(Node* node, Node* root, const string &currentProc) {
    if (!node || has_error)
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
        if (!t.empty() && !has_error) {
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
                string var_type = symbol_tables[currProc][node->tokens[1]];
                node->line += " : " + var_type;
                node->type = var_type;
            }
        }
    }
    
    for (Node* child : node->children) {
        annotate_tree(child, root, currProc);
        if (has_error) return;
    }
}

void print_tree(Node* node) {
    if (!node)
        return;
    cout << node->line << endl;
    for (Node* child : node->children)
        print_tree(child);
}

int main() {
    Node* root = parse_tree();
    if (!root) {
        format_error();
        return 0;
    }
    
    collect_procedures(root);
    if (!has_error) process_all_procedures(root);
    if (!has_error) check_returns(root);
    if (!has_error) annotate_tree(root, root, "");
    
    if (!has_error) {
        print_tree(root);
    }
    
    delete root;
    return 0;
}