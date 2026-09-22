#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cctype>
#include <utility>

// Each node in the parse tree stores one production plus its children.
struct Node {
    std::vector<std::string> tokens;  // production line, split into tokens
    std::vector<Node*> children;      // child nodes for non-terminals
    bool hasType = false;             // true if this node was given a type
    std::string typeName;             // "int" or "int*"
};

// Global buffer for all input lines, already tokenized by spaces.
std::vector<std::vector<std::string>> inputTokens;

// Current index into inputTokens used while reconstructing the parse tree.
int inputIndex = 0;

// Root pointer of the parse tree so it can be freed on success or error.
Node* rootPtr = nullptr;

// Type lattice used in this checker.
enum Type { INT_T, PTR_T, BAD_T };

// Exception type used to abort on semantic/type errors.
struct SemanticError {};

// ------------------------- Custom Variable Symbol Table -------------------------

// Single variable entry: a name and its associated type.
struct VarEntry {
    std::string name;
    Type type;
};

// Small "dictionary" implemented with a vector of VarEntry.
struct VarTable {
    std::vector<VarEntry> entries;
};

VarTable varTable;

// Remove all variable bindings for a new scope.
void clearVarTable() {
    varTable.entries.clear();
}

// Check if a variable name is already present.
bool varExists(const std::string &name) {
    std::size_t i = 0;
    while (i < varTable.entries.size()) {
        if (varTable.entries[i].name == name) {
            return true;
        }
        ++i;
    }
    return false;
}

// Insert a new variable binding.
void insertVar(const std::string &name, Type t) {
    VarEntry e;
    e.name = name;
    e.type = t;
    varTable.entries.push_back(e);
}

// Lookup the type of a variable; caller *must* ensure it exists.
Type lookupVar(const std::string &name) {
    std::size_t i = 0;
    while (i < varTable.entries.size()) {
        if (varTable.entries[i].name == name) {
            return varTable.entries[i].type;
        }
        ++i;
    }
    // If not found, this is a semantic error.
    throw SemanticError();
}

// ------------------------- Custom Procedure Symbol Table -------------------------

// Procedure info: parameter types; return type is always int in WLP4.
struct ProcInfo {
    std::vector<Type> paramTypes;
};

// One entry in the procedure table.
struct ProcEntry {
    std::string name;
    ProcInfo info;
};

// Another simple "dictionary" for procedures.
struct ProcTable {
    std::vector<ProcEntry> entries;
};

ProcTable procTable;

// Remove all procedure entries.
void clearProcTable() {
    procTable.entries.clear();
}

// Check if a procedure already exists.
bool procExists(const std::string &name) {
    std::size_t i = 0;
    while (i < procTable.entries.size()) {
        if (procTable.entries[i].name == name) {
            return true;
        }
        ++i;
    }
    return false;
}

// Insert a new procedure.
void insertProc(const std::string &name, const ProcInfo &info) {
    ProcEntry entry;
    entry.name = name;
    entry.info = info;
    procTable.entries.push_back(entry);
}

// Find procedure info; return pointer or nullptr if missing.
ProcInfo* findProc(const std::string &name) {
    std::size_t i = 0;
    while (i < procTable.entries.size()) {
        if (procTable.entries[i].name == name) {
            return &procTable.entries[i].info;
        }
        ++i;
    }
    return nullptr;
}

// Track whether the mandatory `wain` function has been seen.
bool wainSeen = false;

// ------------------------- Utility Helpers -------------------------

// Tokenize a line from wlp4parse output into individual strings.
std::vector<std::string> tokenizeInputLine(const std::string &line) {
    std::vector<std::string> result;
    std::istringstream iss(line);
    std::string tok;

    while (iss >> tok) {
        result.push_back(tok);
    }
    return result;
}

// Check if a grammar symbol is terminal (convention: starts with uppercase).
bool isTerminalSymbol(const std::string &s) {
    if (s.empty()) {
        return false;
    }
    return std::isupper(static_cast<unsigned char>(s[0])) != 0;
}

// Translate internal type enum to the textual representation used in output.
std::string typeToString(Type t) {
    if (t == INT_T) {
        return "int";
    } else if (t == PTR_T) {
        return "int*";
    }
    return "bad";
}

// Forward declarations for functions that are mutually recursive.
void freeParseTree(Node* node);
[[noreturn]] void signalSemanticError();
Type inferType(Node* node);
Type inferLvalueType(Node* node);
void checkStatements(Node* stmts);

// ------------------------- Parse Tree Reconstruction & Cleanup -------------------------

// Recursively rebuild the parse tree from inputTokens.
Node* rebuildParseTree() {
    Node* node = new Node();
    node->tokens = inputTokens[inputIndex];
    ++inputIndex;

    // Right-hand side symbols of the production on this line.
    std::vector<std::string> rhs;
    std::size_t i = 1;
    while (i < node->tokens.size()) {
        rhs.push_back(node->tokens[i]);
        ++i;
    }

    // For each symbol, decide if it's a terminal or non-terminal.
    std::size_t j = 0;
    while (j < rhs.size()) {
        const std::string &sym = rhs[j];
        if (sym == ".EMPTY") {
            ++j;
            continue;
        }
        if (isTerminalSymbol(sym)) {
            Node* leaf = new Node();
            leaf->tokens = inputTokens[inputIndex];
            ++inputIndex;
            node->children.push_back(leaf);
        } else {
            node->children.push_back(rebuildParseTree());
        }
        ++j;
    }

    return node;
}

// Recursively free the entire parse tree to avoid memory leaks.
void freeParseTree(Node* node) {
    if (node == nullptr) {
        return;
    }
    std::size_t i = 0;
    while (i < node->children.size()) {
        freeParseTree(node->children[i]);
        ++i;
    }
    delete node;
}

// On any semantic error, free the tree, print "ERROR", and throw.
[[noreturn]] void signalSemanticError() {
    if (rootPtr != nullptr) {
        freeParseTree(rootPtr);
        rootPtr = nullptr;
    }
    std::cerr << "ERROR" << std::endl;
    throw SemanticError();
}

// ------------------------- Declarations & Symbol Table Logic -------------------------

// Read a dcl node: dcl -> type ID
// Returns (declared type, variable name) and annotates the ID node.
std::pair<Type, std::string> readDeclaration(Node* dcl) {
    Node* typeNode = dcl->children[0];
    Node* idNode   = dcl->children[1];

    Type t;
    // type -> INT
    // type -> INT STAR
    if (typeNode->tokens.size() == 2) {
        t = INT_T;
    } else {
        t = PTR_T;
    }

    std::string name = idNode->tokens[1];
    idNode->hasType  = true;
    idNode->typeName = (t == INT_T ? "int" : "int*");
    return std::make_pair(t, name);
}

// Process dcls (possibly recursive sequence of declarations with initializers).
// dcls -> .EMPTY
// dcls -> dcls dcl BECOMES NUM SEMI
// dcls -> dcls dcl BECOMES NULL SEMI
void buildDcls(Node* dcls) {
    if (dcls->tokens.size() == 2 && dcls->tokens[1] == ".EMPTY") {
        return;
    }

    Node* prevDcls = dcls->children[0];
    Node* dcl      = dcls->children[1];
    Node* initNode = dcls->children[3];

    buildDcls(prevDcls);

    std::pair<Type, std::string> info = readDeclaration(dcl);
    Type declaredType = info.first;
    const std::string &name = info.second;

    if (varExists(name)) {
        signalSemanticError();
    }

    Type initType = BAD_T;
    if (initNode->tokens[0] == "NUM") {
        initType          = INT_T;
        initNode->hasType = true;
        initNode->typeName = "int";
    } else if (initNode->tokens[0] == "NULL") {
        initType           = PTR_T;
        initNode->hasType  = true;
        initNode->typeName = "int*";
    } else {
        signalSemanticError();
    }

    if (declaredType != initType) {
        signalSemanticError();
    }

    insertVar(name, declaredType);
}

// ------------------------- Procedure Call & Parameters -------------------------

// Check a call to a procedure given its name and argument types.
Type checkCall(const std::string &name, const std::vector<Type> &argTypes) {
    ProcInfo* info = findProc(name);
    if (info == nullptr) {
        signalSemanticError();
    }

    const std::vector<Type> &params = info->paramTypes;
    if (params.size() != argTypes.size()) {
        signalSemanticError();
    }

    std::size_t i = 0;
    while (i < params.size()) {
        if (params[i] != argTypes[i]) {
            signalSemanticError();
        }
        ++i;
    }
    // All procedures return int.
    return INT_T;
}

// Collect parameter declarations from paramlist into paramTypes and var table.
// paramlist -> dcl
// paramlist -> dcl COMMA paramlist
void collectParamlist(Node* pl, std::vector<Type> &paramTypes) {
    if (pl->tokens.size() == 2) {
        Node* dcl = pl->children[0];
        std::pair<Type, std::string> info = readDeclaration(dcl);
        Type t = info.first;
        const std::string &name = info.second;
        if (varExists(name)) {
            signalSemanticError();
        }
        insertVar(name, t);
        paramTypes.push_back(t);
        return;
    }

    if (pl->tokens.size() == 4) {
        Node* dcl  = pl->children[0];
        Node* rest = pl->children[2];
        std::pair<Type, std::string> info = readDeclaration(dcl);
        Type t = info.first;
        const std::string &name = info.second;
        if (varExists(name)) {
            signalSemanticError();
        }
        insertVar(name, t);
        paramTypes.push_back(t);
        collectParamlist(rest, paramTypes);
        return;
    }

    signalSemanticError();
}

// Handle params node: either .EMPTY or paramlist.
void processParams(Node* paramsNode, std::vector<Type> &paramTypes) {
    if (paramsNode->tokens.size() == 2 && paramsNode->tokens[1] == ".EMPTY") {
        return;
    }
    if (paramsNode->tokens.size() == 2 && paramsNode->tokens[1] == "paramlist") {
        collectParamlist(paramsNode->children[0], paramTypes);
        return;
    }
    signalSemanticError();
}

// Collect argument types from arglist recursively.
void collectArgs(Node* argNode, std::vector<Type> &argTypes) {
    if (argNode->tokens.size() == 2) {
        Type t = inferType(argNode->children[0]);
        argTypes.push_back(t);
        return;
    }
    if (argNode->tokens.size() == 4) {
        Type t = inferType(argNode->children[0]);
        argTypes.push_back(t);
        collectArgs(argNode->children[2], argTypes);
        return;
    }
    signalSemanticError();
}

// ------------------------- Lvalue Typing -------------------------

// Compute the type of an lvalue (used directly and from factor).
Type inferLvalueType(Node* u) {
    // lvalue -> ID
    if (u->tokens.size() == 2 && u->tokens[1] == "ID") {
        Node* leaf = u->children[0];
        std::string name = leaf->tokens[1];

        Type t;
        try {
            t = lookupVar(name);
        } catch (SemanticError &) {
            signalSemanticError();
        }

        std::string tname = (t == INT_T ? "int" : "int*");
        leaf->hasType  = true;
        leaf->typeName = tname;
        u->hasType     = true;
        u->typeName    = tname;
        return t;
    }

    // lvalue -> STAR factor
    if (u->tokens.size() == 3 && u->tokens[1] == "STAR") {
        Node* fact = u->children[1];
        Type tf = inferType(fact);
        if (tf != PTR_T) {
            signalSemanticError();
        }
        u->hasType  = true;
        u->typeName = "int";
        return INT_T;
    }

    // lvalue -> LPAREN lvalue RPAREN
    if (u->tokens.size() == 4 && u->tokens[1] == "LPAREN") {
        Node* inner = u->children[1];
        Type t = inferLvalueType(inner);
        std::string tname = (t == INT_T ? "int" : "int*");
        u->hasType  = true;
        u->typeName = tname;
        return t;
    }

    signalSemanticError();
    return BAD_T;
}

// ------------------------- Expression / Factor Typing -------------------------

// Main type inference function for expr/term/factor/lvalue nodes.
Type inferType(Node* u) {
    std::string lhs = u->tokens[0];

    // ---------------- expr ----------------
    if (lhs == "expr") {
        // expr -> term
        if (u->tokens.size() == 2) {
            Type t = inferType(u->children[0]);
            std::string tname = (t == INT_T ? "int" : "int*");
            u->hasType  = true;
            u->typeName = tname;
            return t;
        }

        // expr -> expr PLUS term | expr MINUS term
        if (u->tokens.size() == 4) {
            Node* left  = u->children[0];
            Node* right = u->children[2];
            std::string op = u->tokens[2];

            Type t1 = inferType(left);
            Type t2 = inferType(right);

            Type res = BAD_T;

            if (op == "PLUS") {
                if (t1 == INT_T && t2 == INT_T) {
                    res = INT_T;
                } else if ((t1 == PTR_T && t2 == INT_T) ||
                           (t1 == INT_T && t2 == PTR_T)) {
                    res = PTR_T;
                } else {
                    signalSemanticError();
                }
            } else if (op == "MINUS") {
                if (t1 == INT_T && t2 == INT_T) {
                    res = INT_T;
                } else if (t1 == PTR_T && t2 == INT_T) {
                    res = PTR_T;
                } else if (t1 == PTR_T && t2 == PTR_T) {
                    res = INT_T;
                } else {
                    signalSemanticError();
                }
            } else {
                signalSemanticError();
            }

            std::string tname = (res == INT_T ? "int" : "int*");
            u->hasType  = true;
            u->typeName = tname;
            return res;
        }
        signalSemanticError();
    }

    // ---------------- term ----------------
    if (lhs == "term") {
        // term -> factor
        if (u->tokens.size() == 2) {
            Type t = inferType(u->children[0]);
            std::string tname = (t == INT_T ? "int" : "int*");
            u->hasType  = true;
            u->typeName = tname;
            return t;
        }

        // term -> term STAR/SLASH/PCT factor
        if (u->tokens.size() == 4) {
            Node* left  = u->children[0];
            Node* right = u->children[2];

            Type t1 = inferType(left);
            Type t2 = inferType(right);

            if (t1 != INT_T || t2 != INT_T) {
                signalSemanticError();
            }

            u->hasType  = true;
            u->typeName = "int";
            return INT_T;
        }
        signalSemanticError();
    }

    // ---------------- factor ----------------
    if (lhs == "factor") {
        // factor -> NUM
        if (u->tokens.size() == 2 && u->tokens[1] == "NUM") {
            Node* leaf = u->children[0];
            leaf->hasType  = true;
            leaf->typeName = "int";
            u->hasType     = true;
            u->typeName    = "int";
            return INT_T;
        }

        // factor -> NULL
        if (u->tokens.size() == 2 && u->tokens[1] == "NULL") {
            Node* leaf = u->children[0];
            leaf->hasType  = true;
            leaf->typeName = "int*";
            u->hasType     = true;
            u->typeName    = "int*";
            return PTR_T;
        }

        // factor -> ID
        if (u->tokens.size() == 2 && u->tokens[1] == "ID") {
            Node* leaf = u->children[0];
            std::string name = leaf->tokens[1];

            Type t;
            try {
                t = lookupVar(name);
            } catch (SemanticError &) {
                signalSemanticError();
            }

            std::string tname = (t == INT_T ? "int" : "int*");
            leaf->hasType  = true;
            leaf->typeName = tname;
            u->hasType     = true;
            u->typeName    = tname;
            return t;
        }

        // factor -> LPAREN expr RPAREN
        if (u->tokens.size() == 4 && u->tokens[1] == "LPAREN") {
            Node* inner = u->children[1];
            Type t = inferType(inner);
            std::string tname = (t == INT_T ? "int" : "int*");
            u->hasType  = true;
            u->typeName = tname;
            return t;
        }

        // factor -> AMP lvalue
        if (u->tokens.size() == 3 && u->tokens[1] == "AMP") {
            Node* lv = u->children[1];
            Type t = inferLvalueType(lv);
            if (t != INT_T) {
                signalSemanticError();
            }
            u->hasType  = true;
            u->typeName = "int*";
            return PTR_T;
        }

        // factor -> STAR factor
        if (u->tokens.size() == 3 && u->tokens[1] == "STAR") {
            Node* fact = u->children[1];
            Type t = inferType(fact);
            if (t != PTR_T) {
                signalSemanticError();
            }
            u->hasType  = true;
            u->typeName = "int";
            return INT_T;
        }

        // factor -> NEW INT LBRACK expr RBRACK
        if (u->tokens.size() == 6 && u->tokens[1] == "NEW") {
            Node* lenExpr = u->children[3];
            Type t = inferType(lenExpr);
            if (t != INT_T) {
                signalSemanticError();
            }
            u->hasType  = true;
            u->typeName = "int*";
            return PTR_T;
        }

        // factor -> ID LPAREN RPAREN    (procedure call, no args)
        if (u->tokens.size() == 4 &&
            u->tokens[1] == "ID" &&
            u->tokens[2] == "LPAREN") {

            Node* idLeaf = u->children[0];
            std::string name = idLeaf->tokens[1];
            std::vector<Type> args;
            Type ret = checkCall(name, args);
            (void)ret;
            u->hasType  = true;
            u->typeName = "int";
            return INT_T;
        }

        // factor -> ID LPAREN arglist RPAREN
        if (u->tokens.size() == 5 &&
            u->tokens[1] == "ID" &&
            u->tokens[2] == "LPAREN") {

            Node* idLeaf      = u->children[0];
            Node* arglistNode = u->children[2];
            std::string name  = idLeaf->tokens[1];
            std::vector<Type> args;
            collectArgs(arglistNode, args);
            Type ret = checkCall(name, args);
            (void)ret;
            u->hasType  = true;
            u->typeName = "int";
            return INT_T;
        }

        // factor -> GETCHAR LPAREN RPAREN
        if (u->tokens.size() == 4 && u->tokens[1] == "GETCHAR") {
            u->hasType  = true;
            u->typeName = "int";
            return INT_T;
        }

        signalSemanticError();
    }

    // lvalue handled through this function when necessary.
    if (lhs == "lvalue") {
        return inferLvalueType(u);
    }

    signalSemanticError();
    return BAD_T;
}

// ------------------------- Tests and Statements -------------------------

// Ensure test operands and operators are type-correct.
void checkTest(Node* test) {
    Node* e1 = test->children[0];
    Node* e2 = test->children[2];
    std::string op = test->tokens[2];

    Type t1 = inferType(e1);
    Type t2 = inferType(e2);

    if (op == "EQ" || op == "NE") {
        if (t1 != t2) {
            signalSemanticError();
        }
    } else {
        if (t1 != INT_T || t2 != INT_T) {
            signalSemanticError();
        }
    }
}

// Forward declaration to allow mutual recursion.
void checkStatement(Node* stmt);

// Check a sequence of statements (statements non-terminal).
void checkStatements(Node* stmts) {
    // statements -> .EMPTY
    if (stmts->tokens.size() == 2 && stmts->tokens[1] == ".EMPTY") {
        return;
    }

    // statements -> statements statement
    if (stmts->tokens.size() == 3 && stmts->tokens[1] == "statements") {
        Node* prev = stmts->children[0];
        Node* st   = stmts->children[1];
        checkStatements(prev);
        checkStatement(st);
        return;
    }

    signalSemanticError();
}

// Check a single statement node.
void checkStatement(Node* stmt) {
    // statement -> lvalue BECOMES expr SEMI
    if (stmt->tokens.size() == 5 && stmt->tokens[1] == "lvalue") {
        Node* lv = stmt->children[0];
        Node* e  = stmt->children[2];
        Type tl = inferLvalueType(lv);
        Type tr = inferType(e);
        if (tl != tr) {
            signalSemanticError();
        }
        return;
    }

    // statement -> IF LPAREN test RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE
    if (stmt->tokens.size() == 12 && stmt->tokens[1] == "IF") {
        Node* testNode  = stmt->children[2];
        Node* thenStmts = stmt->children[5];
        Node* elseStmts = stmt->children[9];
        checkTest(testNode);
        checkStatements(thenStmts);
        checkStatements(elseStmts);
        return;
    }

    // statement -> WHILE LPAREN test RPAREN LBRACE statements RBRACE
    if (stmt->tokens.size() == 8 && stmt->tokens[1] == "WHILE") {
        Node* testNode = stmt->children[2];
        Node* body     = stmt->children[5];
        checkTest(testNode);
        checkStatements(body);
        return;
    }

    // statement -> PRINTLN LPAREN expr RPAREN SEMI
    if (stmt->tokens.size() == 6 && stmt->tokens[1] == "PRINTLN") {
        Node* e = stmt->children[2];
        Type t = inferType(e);
        if (t != INT_T) {
            signalSemanticError();
        }
        return;
    }

    // statement -> PUTCHAR LPAREN expr RPAREN SEMI
    if (stmt->tokens.size() == 6 && stmt->tokens[1] == "PUTCHAR") {
        Node* e = stmt->children[2];
        Type t = inferType(e);
        if (t != INT_T) {
            signalSemanticError();
        }
        return;
    }

    // statement -> DELETE LBRACK RBRACK expr SEMI
    if (stmt->tokens.size() == 6 && stmt->tokens[1] == "DELETE") {
        Node* e = stmt->children[3];
        Type t = inferType(e);
        if (t != PTR_T) {
            signalSemanticError();
        }
        return;
    }

    signalSemanticError();
}

// ------------------------- Procedure / main Analysis -------------------------

// Analyze the special main (wain) procedure.
void analyzeMain(Node* mainNode) {
    // main INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE
    Node* dcl1       = mainNode->children[3];
    Node* dcl2       = mainNode->children[5];
    Node* dclsNode   = mainNode->children[8];
    Node* stmtsNode  = mainNode->children[9];
    Node* returnExpr = mainNode->children[11];

    clearVarTable();

    std::pair<Type, std::string> p1 = readDeclaration(dcl1);
    std::pair<Type, std::string> p2 = readDeclaration(dcl2);

    insertVar(p1.second, p1.first);
    if (varExists(p2.second)) {
        signalSemanticError();
    }
    insertVar(p2.second, p2.first);

    // Second parameter of wain must be int.
    if (p2.first != INT_T) {
        signalSemanticError();
    }

    // Ensure there is at most one wain.
    if (procExists("wain")) {
        signalSemanticError();
    }
    ProcInfo info;
    info.paramTypes.push_back(p1.first);
    info.paramTypes.push_back(p2.first);
    insertProc("wain", info);

    buildDcls(dclsNode);
    checkStatements(stmtsNode);

    Type retType = inferType(returnExpr);
    if (retType != INT_T) {
        signalSemanticError();
    }

    wainSeen = true;
}

// Analyze all other (non-main) procedures.
void analyzeProcedure(Node* procNode) {
    // procedure INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE
    Node* idLeaf = procNode->children[1];
    std::string name = idLeaf->tokens[1];

    clearVarTable();
    std::vector<Type> paramTypes;

    Node* paramsNode = procNode->children[3];
    processParams(paramsNode, paramTypes);

    if (procExists(name)) {
        signalSemanticError();
    }
    ProcInfo info;
    info.paramTypes = paramTypes;
    insertProc(name, info);

    Node* dclsNode   = procNode->children[6];
    Node* stmtsNode  = procNode->children[7];
    Node* returnExpr = procNode->children[9];

    buildDcls(dclsNode);
    checkStatements(stmtsNode);

    Type retType = inferType(returnExpr);
    if (retType != INT_T) {
        signalSemanticError();
    }
}

// Walk the top-level procedures non-terminal.
void processProcedures(Node* node) {
    // procedures -> main
    if (node->tokens.size() == 2 && node->tokens[1] == "main") {
        Node* mainNode = node->children[0];
        analyzeMain(mainNode);
        return;
    }

    // procedures -> procedure procedures
    if (node->tokens.size() == 3 &&
        node->tokens[1] == "procedure" &&
        node->tokens[2] == "procedures") {
        Node* procNode = node->children[0];
        Node* rest     = node->children[1];
        analyzeProcedure(procNode);
        processProcedures(rest);
        return;
    }

    signalSemanticError();
}

// ------------------------- Printing Annotated Tree -------------------------

// Pre-order print of the parse tree, attaching : type for typed nodes.
void printAnnotatedTree(Node* u) {
    std::size_t i = 0;
    while (i < u->tokens.size()) {
        if (i != 0) {
            std::cout << ' ';
        }
        std::cout << u->tokens[i];
        ++i;
    }

    if (u->hasType) {
        std::cout << " : " << u->typeName;
    }
    std::cout << "\n";

    std::size_t k = 0;
    while (k < u->children.size()) {
        printAnnotatedTree(u->children[k]);
        ++k;
    }
}

// ------------------------- main -------------------------

int main() {
    try {
        std::string line;
        // Read all lines from stdin (output from wlp4parse).
        while (std::getline(std::cin, line)) {
            inputTokens.push_back(tokenizeInputLine(line));
        }

        if (inputTokens.empty()) {
            signalSemanticError();
        }

        inputIndex = 0;
        rootPtr = rebuildParseTree();

        clearProcTable();
        wainSeen = false;

        // start -> BOF procedures EOF
        Node* proceduresNode = rootPtr->children[1];
        processProcedures(proceduresNode);

        if (!wainSeen) {
            signalSemanticError();
        }

        printAnnotatedTree(rootPtr);
        freeParseTree(rootPtr);
        rootPtr = nullptr;
    } catch (SemanticError &) {
        // Error already reported; tree freed inside signalSemanticError.
    }

    return 0;
}
