#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <utility>

//Parse-tree node

struct Node {
    std::string rawLine;                 // Original line from wlp4type
    std::vector<std::string> symbolList; // Tokens in this line
    std::vector<Node*> childNodes;       // Children in the parse tree
    std::string nodeType;                // Type annotation from wlp4type (e.g., "int", "int*")

    ~Node() {
        std::size_t i = 0;
        while (i < childNodes.size()) {
            delete childNodes[i];
            ++i;
        }
    }
};

//Simple symbol-table entry

struct VarEntry {
    std::string varIdentifier; // Variable name
    int stackOffset;           // Offset from $29
};

std::vector<VarEntry> gSymbolTable;   // variable → stack offset
std::vector<std::string> gTerminals;  // list of terminal symbols
int gNextStackOffset = 0;             // next free offset (more negative)
int gNextLabelId = 0;                 // for unique labels

// Forward decls of codegen functions
void generateExpressionCode(Node* astNode);
void generateStatementListCode(Node* stmtListNode);
void generateSingleStatementCode(Node* stmtNode);
void generateDeclarationCode(Node* declsNode);
void generateTestBranchCode(Node* testAstNode,
                            const std::string &labelTrue,
                            const std::string &labelFalse);
void generateArglistToRegs(Node* arglistNode, int &argIndex);

//  Helpers: terminals, symbol table, labels

void setupTerminalList() {
    const char* terminalNameArray[] = {
        "BOF", "INT", "WAIN", "LPAREN", "RPAREN", "COMMA",
        "LBRACE", "RBRACE", "RETURN", "SEMI", "STAR",
        "ID", "NUM", "NULL", "EOF", "BECOMES",
        "PLUS", "MINUS", "SLASH", "PCT",
        "IF", "ELSE", "WHILE", "EQ", "NE", "LT", "GT", "LE", "GE",
        "PUTCHAR", "PRINTLN", "GETCHAR"
    };
    std::size_t n = sizeof(terminalNameArray) / sizeof(terminalNameArray[0]);
    std::size_t i = 0;
    while (i < n) {
        gTerminals.push_back(std::string(terminalNameArray[i]));
        ++i;
    }
}

bool checkTerminalSymbol(const std::string &symbolToken) {
    std::size_t i = 0;
    while (i < gTerminals.size()) {
        if (gTerminals[i] == symbolToken) return true;
        ++i;
    }
    return false;
}

bool lookupVariableOffset(const std::string &name, int &foundOffset) {
    std::size_t i = 0;
    while (i < gSymbolTable.size()) {
        if (gSymbolTable[i].varIdentifier == name) {
            foundOffset = gSymbolTable[i].stackOffset;
            return true;
        }
        ++i;
    }
    return false;
}

void registerVariableSymbol(const std::string &name, int offset) {
    int dummy = 0;
    if (!lookupVariableOffset(name, dummy)) {
        VarEntry e;
        e.varIdentifier = name;
        e.stackOffset = offset;
        gSymbolTable.push_back(e);
    }
}

std::string generateUniqueLabel(const std::string &base) {
    std::string lbl = base;
    lbl += std::to_string(gNextLabelId);
    ++gNextLabelId;
    return lbl;
}

//  Build parse tree from stdin

Node* constructParseTree() {
    std::string line;
    if (!std::getline(std::cin, line)) return nullptr;

    Node* node = new Node();
    node->rawLine = line;

    std::istringstream iss(line);
    std::string tok;
    while (iss >> tok) node->symbolList.push_back(tok);

    // pick up type annotation after ":"
    for (std::size_t i = 0; i + 1 < node->symbolList.size(); ++i) {
        if (node->symbolList[i] == ":") {
            node->nodeType = node->symbolList[i + 1];
            break;
        }
    }

    if (node->symbolList.empty()) return node;

    if (checkTerminalSymbol(node->symbolList[0]) ||
        (node->symbolList.size() == 2 && node->symbolList[1] == ".EMPTY")) {
        return node; // leaf
    }

    std::size_t idx = 1;
    while (idx < node->symbolList.size()) {
        if (node->symbolList[idx] == ":" ||
            (idx > 0 && node->symbolList[idx - 1] == ":")) {
            ++idx;
            continue;
        }
        Node* child = constructParseTree();
        if (child) node->childNodes.push_back(child);
        ++idx;
    }
    return node;
}

//  Declarations

std::pair<std::string,std::string> parseDeclarationNode(Node* declNode) {
    std::string typeStr = "int";
    std::string name;

    if (declNode) {
        if (!declNode->nodeType.empty()) typeStr = declNode->nodeType;
        if (declNode->childNodes.size() > 1) {
            Node* idNode = declNode->childNodes[1];
            if (idNode && idNode->symbolList.size() > 1) {
                name = idNode->symbolList[1];
            }
        }
    }
    return std::pair<std::string,std::string>(typeStr, name);
}

void generateDeclarationCode(Node* declsNode) {
    if (!declsNode) return;

    if (declsNode->childNodes.empty() ||
        (declsNode->symbolList.size() > 1 &&
         declsNode->symbolList[1] == ".EMPTY")) {
        return;
    }

    // dcls -> dcls dcl BECOMES NUM SEMI
    if (declsNode->childNodes.size() == 5) {
        generateDeclarationCode(declsNode->childNodes[0]);

        Node* dclNode  = declsNode->childNodes[1];
        Node* numNode  = declsNode->childNodes[3];

        if (dclNode && dclNode->childNodes.size() == 2) {
            std::pair<std::string,std::string> info = parseDeclarationNode(dclNode);
            std::string name = info.second;

            if (!name.empty()) {
                int newOffset = gNextStackOffset;
                registerVariableSymbol(name, newOffset);
                gNextStackOffset -= 4;

                if (numNode->symbolList.size() > 1 &&
                    numNode->symbolList[1] == "0") {
                    std::cout << "sw $0, " << newOffset << "($29)\n";
                } else {
                    std::cout << "lis $3\n";
                    std::cout << ".word " << numNode->symbolList[1] << "\n";
                    std::cout << "sw $3, " << newOffset << "($29)\n";
                }
                std::cout << "sub $30, $30, $4\n";
            }
        }
    }
}

//  Expressions (expr / term / factor / lvalue)

void generateArglistToRegs(Node* arglistNode, int &argIndex); // forward

void generateExpressionCode(Node* astNode) {
    if (!astNode || astNode->symbolList.empty()) return;

    std::string label = astNode->symbolList[0];

    if (label == "expr") {
        if (astNode->childNodes.size() == 1) {
            generateExpressionCode(astNode->childNodes[0]);
        } else if (astNode->childNodes.size() == 3 &&
                   !astNode->childNodes[0]->symbolList.empty() &&
                   astNode->childNodes[0]->symbolList[0] == "expr") {

            generateExpressionCode(astNode->childNodes[0]);

            std::cout << "sw $3, -4($30)\n";
            std::cout << "sub $30, $30, $4\n";

            generateExpressionCode(astNode->childNodes[2]);

            std::cout << "add $30, $30, $4\n";
            std::cout << "lw $5, -4($30)\n";

            if (astNode->childNodes[1]->symbolList[0] == "PLUS") {
                std::cout << "add $3, $5, $3\n";
            } else if (astNode->childNodes[1]->symbolList[0] == "MINUS") {
                std::cout << "sub $3, $5, $3\n";
            }
        }
    } else if (label == "term") {
        if (astNode->childNodes.size() == 1) {
            generateExpressionCode(astNode->childNodes[0]);
        } else if (astNode->childNodes.size() == 3 &&
                   !astNode->childNodes[0]->symbolList.empty() &&
                   astNode->childNodes[0]->symbolList[0] == "term") {

            generateExpressionCode(astNode->childNodes[0]);

            std::cout << "sw $3, -4($30)\n";
            std::cout << "sub $30, $30, $4\n";

            generateExpressionCode(astNode->childNodes[2]);

            std::cout << "add $30, $30, $4\n";
            std::cout << "lw $5, -4($30)\n";

            std::string op = astNode->childNodes[1]->symbolList[0];
            if (op == "STAR") {
                std::cout << "mult $5, $3\n";
                std::cout << "mflo $3\n";
            } else if (op == "SLASH") {
                std::cout << "div $5, $3\n";
                std::cout << "mflo $3\n";
            } else if (op == "PCT") {
                std::cout << "div $5, $3\n";
                std::cout << "mfhi $3\n";
            }
        }
    } else if (label == "factor") {
        // constants / variables / getchar / parentheses / procedure calls
        if (astNode->childNodes.size() == 1) {
            Node* c = astNode->childNodes[0];
            if (c->symbolList[0] == "ID") {
                std::string name = c->symbolList[1];
                int off = 0;
                if (lookupVariableOffset(name, off)) {
                    std::cout << "lw $3, " << off << "($29)\n";
                } else {
                    std::cout << "add $3, $0, $0\n"; // defensive
                }
            } else if (c->symbolList[0] == "NUM") {
                std::cout << "lis $3\n";
                std::cout << ".word " << c->symbolList[1] << "\n";
            }
        }
        // zero-argument procedure call: factor -> ID LPAREN RPAREN
        else if (astNode->childNodes.size() == 3 &&
                 astNode->childNodes[0]->symbolList[0] == "ID" &&
                 astNode->childNodes[1]->symbolList[0] == "LPAREN" &&
                 astNode->childNodes[2]->symbolList[0] == "RPAREN") {

            std::string procName = astNode->childNodes[0]->symbolList[1];

            // save caller's $1,$2,$31
            std::cout << "sw $1, -4($30)\n";
            std::cout << "sub $30, $30, $4\n";
            std::cout << "sw $2, -4($30)\n";
            std::cout << "sub $30, $30, $4\n";
            std::cout << "sw $31, -4($30)\n";
            std::cout << "sub $30, $30, $4\n";

            std::cout << "lis $5\n";
            std::cout << ".word " << procName << "\n";
            std::cout << "jalr $5\n";

            // restore $31, $2, $1
            std::cout << "add $30, $30, $4\n";
            std::cout << "lw $31, -4($30)\n";
            std::cout << "add $30, $30, $4\n";
            std::cout << "lw $2, -4($30)\n";
            std::cout << "add $30, $30, $4\n";
            std::cout << "lw $1, -4($30)\n";
        }
        // call with arguments: factor -> ID LPAREN arglist RPAREN
        else if (astNode->childNodes.size() == 4 &&
                 astNode->childNodes[0]->symbolList[0] == "ID" &&
                 astNode->childNodes[1]->symbolList[0] == "LPAREN" &&
                 astNode->childNodes[3]->symbolList[0] == "RPAREN") {

            std::string procName = astNode->childNodes[0]->symbolList[1];
            Node* arglistNode = astNode->childNodes[2];

            // save caller's $1,$2,$31
            std::cout << "sw $1, -4($30)\n";
            std::cout << "sub $30, $30, $4\n";
            std::cout << "sw $2, -4($30)\n";
            std::cout << "sub $30, $30, $4\n";
            std::cout << "sw $31, -4($30)\n";
            std::cout << "sub $30, $30, $4\n";

            int argIndex = 0;
            generateArglistToRegs(arglistNode, argIndex); // fills $1,$2,$3

            std::cout << "lis $5\n";
            std::cout << ".word " << procName << "\n";
            std::cout << "jalr $5\n";

            // restore caller registers
            std::cout << "add $30, $30, $4\n";
            std::cout << "lw $31, -4($30)\n";
            std::cout << "add $30, $30, $4\n";
            std::cout << "lw $2, -4($30)\n";
            std::cout << "add $30, $30, $4\n";
            std::cout << "lw $1, -4($30)\n";
        }
        // getchar
        else if (astNode->childNodes.size() == 3 &&
                 astNode->childNodes[0]->symbolList[0] == "GETCHAR") {
            std::cout << "lis $5\n";
            std::cout << ".word 0xffff0004\n";
            std::cout << "lw $3, 0($5)\n";
        }
        // parentheses
        else if (astNode->childNodes.size() == 3 &&
                 astNode->childNodes[0]->symbolList[0] == "LPAREN" &&
                 astNode->childNodes[2]->symbolList[0] == "RPAREN") {
            generateExpressionCode(astNode->childNodes[1]);
        }
    } else if (label == "lvalue") {
        if (astNode->childNodes.size() == 1 &&
            astNode->childNodes[0]->symbolList[0] == "ID") {
            std::string name = astNode->childNodes[0]->symbolList[1];
            int off = 0;
            if (lookupVariableOffset(name, off)) {
                std::cout << "lw $3, " << off << "($29)\n";
            }
        } else if (astNode->childNodes.size() == 3 &&
                   astNode->childNodes[0]->symbolList[0] == "LPAREN" &&
                   astNode->childNodes[2]->symbolList[0] == "RPAREN") {
            generateExpressionCode(astNode->childNodes[1]);
        }
    }
}

// arglist -> expr | expr COMMA arglist
void generateArglistToRegs(Node* arglistNode, int &argIndex) {
    if (!arglistNode) return;
    if (arglistNode->symbolList.empty()) return;

    if (arglistNode->symbolList[0] == "arglist") {
        if (arglistNode->childNodes.size() == 1) {
            // arglist -> expr
            generateExpressionCode(arglistNode->childNodes[0]);
            if (argIndex == 0) {
                std::cout << "add $1, $3, $0\n";
            } else if (argIndex == 1) {
                std::cout << "add $2, $3, $0\n";
            } else if (argIndex == 2) {
                std::cout << "add $3, $3, $0\n";
            }
            ++argIndex;
        } else if (arglistNode->childNodes.size() == 3) {
            // arglist -> expr COMMA arglist
            generateExpressionCode(arglistNode->childNodes[0]);
            if (argIndex == 0) {
                std::cout << "add $1, $3, $0\n";
            } else if (argIndex == 1) {
                std::cout << "add $2, $3, $0\n";
            } else if (argIndex == 2) {
                std::cout << "add $3, $3, $0\n";
            }
            ++argIndex;
            generateArglistToRegs(arglistNode->childNodes[2], argIndex);
        }
    } else {
        // Defensive: some trees may skip "arglist" node
        if (!arglistNode->childNodes.empty()) {
            generateExpressionCode(arglistNode->childNodes[0]);
            if (argIndex == 0) {
                std::cout << "add $1, $3, $0\n";
            } else if (argIndex == 1) {
                std::cout << "add $2, $3, $0\n";
            } else if (argIndex == 2) {
                std::cout << "add $3, $3, $0\n";
            }
            ++argIndex;
        }
    }
}

//  Tests (comparisons)

void generateTestBranchCode(Node* testNode,
                            const std::string &labelTrue,
                            const std::string &labelFalse) {
    if (!testNode) return;

    generateExpressionCode(testNode->childNodes[0]);
    std::cout << "sw $3, -4($30)\n";
    std::cout << "sub $30, $30, $4\n";

    generateExpressionCode(testNode->childNodes[2]);
    std::cout << "add $30, $30, $4\n";
    std::cout << "lw $5, -4($30)\n";

    std::string op = testNode->childNodes[1]->symbolList[0];
    if (op == "EQ") {
        std::cout << "beq $5, $3, " << labelTrue << "\n";
        std::cout << "beq $0, $0, " << labelFalse << "\n";
    } else if (op == "NE") {
        std::cout << "bne $5, $3, " << labelTrue << "\n";
        std::cout << "beq $0, $0, " << labelFalse << "\n";
    } else if (op == "LT") {
        std::cout << "slt $3, $5, $3\n";
        std::cout << "bne $3, $0, " << labelTrue << "\n";
        std::cout << "beq $0, $0, " << labelFalse << "\n";
    } else if (op == "LE") {
        std::cout << "slt $3, $3, $5\n";
        std::cout << "beq $3, $0, " << labelTrue << "\n";
        std::cout << "beq $0, $0, " << labelFalse << "\n";
    } else if (op == "GT") {
        std::cout << "slt $3, $3, $5\n";
        std::cout << "bne $3, $0, " << labelTrue << "\n";
        std::cout << "beq $0, $0, " << labelFalse << "\n";
    } else if (op == "GE") {
        std::cout << "slt $3, $5, $3\n";
        std::cout << "beq $3, $0, " << labelTrue << "\n";
        std::cout << "beq $0, $0, " << labelFalse << "\n";
    }
}

// helper for assignments
std::string extractLvalueIdentifier(Node* lvalueNode) {
    if (!lvalueNode) return "";
    if (lvalueNode->childNodes.size() == 1 &&
        lvalueNode->childNodes[0]->symbolList.size() > 1) {
        return lvalueNode->childNodes[0]->symbolList[1];
    }
    if (lvalueNode->childNodes.size() == 3) {
        return extractLvalueIdentifier(lvalueNode->childNodes[1]);
    }
    return "";
}

//  Statements

void generateStatementListCode(Node* stmtsNode) {
    if (!stmtsNode || stmtsNode->childNodes.empty()) return;

    if (stmtsNode->childNodes.size() == 2) {
        generateStatementListCode(stmtsNode->childNodes[0]);
        generateSingleStatementCode(stmtsNode->childNodes[1]);
    }
}

void generateSingleStatementCode(Node* stmtNode) {
    if (!stmtNode) return;

    // assignment: lvalue BECOMES expr SEMI
    if (stmtNode->childNodes.size() == 4) {
        generateExpressionCode(stmtNode->childNodes[2]);
        std::string name = extractLvalueIdentifier(stmtNode->childNodes[0]);

        if (name == "$1") {
            std::cout << "sw $3, -0($29)\n";
        } else if (name == "$2") {
            std::cout << "sw $3, -4($29)\n";
        } else {
            int off = 0;
            if (lookupVariableOffset(name, off)) {
                std::cout << "sw $3, " << off << "($29)\n";
            }
        }
    }
    // if-else
    else if (stmtNode->childNodes.size() == 11) {
        std::string lblIf   = generateUniqueLabel("if");
        std::string lblElse = generateUniqueLabel("else");
        std::string lblEnd  = generateUniqueLabel("endif");

        generateTestBranchCode(stmtNode->childNodes[2], lblIf, lblElse);

        std::cout << lblIf << ":\n";
        generateStatementListCode(stmtNode->childNodes[5]);
        std::cout << "beq $0, $0, " << lblEnd << "\n";

        std::cout << lblElse << ":\n";
        generateStatementListCode(stmtNode->childNodes[9]);
        std::cout << lblEnd << ":\n";
    }
    // while
    else if (stmtNode->childNodes.size() == 7 &&
             stmtNode->childNodes[0]->symbolList[0] == "WHILE") {
        std::string lblCond = generateUniqueLabel("while");
        std::string lblBody = generateUniqueLabel("body");
        std::string lblEnd  = generateUniqueLabel("wend");

        std::cout << lblCond << ":\n";
        generateTestBranchCode(stmtNode->childNodes[2], lblBody, lblEnd);

        std::cout << lblBody << ":\n";
        generateStatementListCode(stmtNode->childNodes[5]);
        std::cout << "beq $0, $0, " << lblCond << "\n";
        std::cout << lblEnd << ":\n";
    }
    // PUTCHAR / PRINTLN
    else if (stmtNode->childNodes.size() == 5) {
        generateExpressionCode(stmtNode->childNodes[2]);

        if (stmtNode->childNodes[0]->symbolList[0] == "PUTCHAR") {
            std::cout << "lis $5\n";
            std::cout << ".word 0xffff000c\n";
            std::cout << "sw $3, 0($5)\n";
        } else if (stmtNode->childNodes[0]->symbolList[0] == "PRINTLN") {
            std::cout << "sw $3, -4($30)\n";
            std::cout << "sub $30, $30, $4\n";
            std::cout << "sw $1, -4($30)\n";
            std::cout << "sub $30, $30, $4\n";

            std::cout << "add $1, $3, $0\n";

            std::cout << "sw $31, -4($30)\n";
            std::cout << "sub $30, $30, $4\n";

            std::cout << "lis $5\n";
            std::cout << ".word print\n";
            std::cout << "jalr $5\n";

            std::cout << "add $30, $30, $4\n";
            std::cout << "lw $31, -4($30)\n";
            std::cout << "add $30, $30, $4\n";
            std::cout << "lw $1, -4($30)\n";
            std::cout << "add $30, $30, $4\n";
            std::cout << "lw $3, -4($30)\n";
        }
    }
}

//  Procedures (A8P1) and wain

void collectParamDclsFromNode(Node* node, std::vector<Node*>& out) {
    if (!node) return;
    if (node->symbolList.empty()) return;

    std::string lab = node->symbolList[0];
    if (lab == "params") {
        if (!node->childNodes.empty()) {
            collectParamDclsFromNode(node->childNodes[0], out);
        }
    } else if (lab == "paramlist") {
        if (node->childNodes.size() == 1 &&
            node->childNodes[0]->symbolList[0] == "dcl") {
            out.push_back(node->childNodes[0]);
        } else if (node->childNodes.size() == 3 &&
                   node->childNodes[0]->symbolList[0] == "dcl") {
            out.push_back(node->childNodes[0]);
            collectParamDclsFromNode(node->childNodes[2], out);
        }
    } else if (lab == "dcl") {
        out.push_back(node);
    }
}

void collectProcedures(Node* proceduresNode,
                       std::vector<Node*>& procs,
                       Node*& mainNode) {
    if (!proceduresNode || proceduresNode->symbolList.empty()) return;
    if (proceduresNode->symbolList[0] != "procedures") return;

    if (proceduresNode->childNodes.size() == 1) {
        Node* only = proceduresNode->childNodes[0];
        if (!only->symbolList.empty() &&
            only->symbolList[0] == "main") {
            mainNode = only;
        }
        return;
    }

    if (proceduresNode->childNodes.size() == 2) {
        Node* first = proceduresNode->childNodes[0];
        Node* rest  = proceduresNode->childNodes[1];

        if (!first->symbolList.empty()) {
            if (first->symbolList[0] == "procedure") {
                procs.push_back(first);
            } else if (first->symbolList[0] == "main") {
                mainNode = first;
            }
        }
        collectProcedures(rest, procs, mainNode);
    }
}

void generateProcedureCode(Node* procNode) {
    if (!procNode) return;
    if (procNode->childNodes.size() < 12) return;

    // procedure -> INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE
    Node* idNode     = procNode->childNodes[1];
    Node* paramsNode = procNode->childNodes[3];
    Node* dclsNode   = procNode->childNodes[6];
    Node* stmtsNode  = procNode->childNodes[7];
    Node* retExpr    = procNode->childNodes[9];

    if (!idNode || idNode->symbolList.size() < 2) return;
    std::string name = idNode->symbolList[1];

    std::cout << name << ":\n";

    gSymbolTable.clear();
    gNextStackOffset = 0;

    // set up frame pointer for this procedure
    std::cout << "sub $29, $30, $4\n";

    // collect parameter dcls
    std::vector<Node*> paramDcls;
    collectParamDclsFromNode(paramsNode, paramDcls);
    int paramCount = static_cast<int>(paramDcls.size());

    // save parameters into frame (support first three via $1,$2,$3)
    for (int i = 0; i < paramCount; ++i) {
        std::pair<std::string,std::string> info = parseDeclarationNode(paramDcls[i]);
        std::string varName = info.second;
        int off = -4 * i; // 0, -4, -8, ...

        registerVariableSymbol(varName, off);

        std::string reg = "$0";
        if (i == 0) reg = "$1";
        else if (i == 1) reg = "$2";
        else if (i == 2) reg = "$3";

        std::cout << "sw " << reg << ", " << off << "($29)\n";
        std::cout << "sub $30, $30, $4\n";
    }

    gNextStackOffset = -4 * paramCount;

    if (dclsNode) generateDeclarationCode(dclsNode);
    if (stmtsNode) generateStatementListCode(stmtsNode);
    if (retExpr)   generateExpressionCode(retExpr);
    else           std::cout << "add $3, $0, $0\n";

    int slotCount = (-gNextStackOffset) / 4;
    int i = 0;
    while (i < slotCount) {
        std::cout << "add $30, $30, $4\n";
        ++i;
    }

    std::cout << "jr $31\n";
}

void generateProgramCode(Node* root) {
    if (!root || root->childNodes.size() < 2) return;

    Node* proceduresNode = root->childNodes[1];

    std::vector<Node*> procs;
    Node* mainNode = nullptr;
    collectProcedures(proceduresNode, procs, mainNode);
    if (!mainNode) return;

    std::cout << ".import print\n";
    std::cout << "lis $4\n";
    std::cout << ".word 4\n";

    std::string wainLabel = "wainstart";
    std::cout << "beq $0, $0, " << wainLabel << "\n";

    // emit non-wain procedures
    for (std::size_t i = 0; i < procs.size(); ++i) {
        generateProcedureCode(procs[i]);
    }

    // -------- wain --------
    std::cout << wainLabel << ":\n";

    // main -> INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE
    if (mainNode->childNodes.size() < 12) return;

    Node* p1Node   = mainNode->childNodes[3];
    Node* p2Node   = mainNode->childNodes[5];
    Node* dclsNode = mainNode->childNodes[8];
    Node* stmtsNode= mainNode->childNodes[9];
    Node* retExpr  = mainNode->childNodes[11];

    gSymbolTable.clear();

    std::cout << "sub $29, $30, $4\n";

    std::pair<std::string,std::string> p1Info = parseDeclarationNode(p1Node);
    std::pair<std::string,std::string> p2Info = parseDeclarationNode(p2Node);
    std::string p1Name = p1Info.second;
    std::string p2Name = p2Info.second;

    std::cout << "sw $1, 0($29)\n";
    std::cout << "sub $30, $30, $4\n";
    std::cout << "sw $2, -4($29)\n";
    std::cout << "sub $30, $30, $4\n";

    registerVariableSymbol(p1Name, 0);
    registerVariableSymbol(p2Name, -4);
    gNextStackOffset = -8;

    if (dclsNode) generateDeclarationCode(dclsNode);
    if (stmtsNode) generateStatementListCode(stmtsNode);
    if (retExpr)   generateExpressionCode(retExpr);

    int slotCount = (-gNextStackOffset) / 4;
    int i = 0;
    while (i < slotCount) {
        std::cout << "add $30, $30, $4\n";
        ++i;
    }

    std::cout << "jr $31\n";
}

//  main

int main() {
    setupTerminalList();
    Node* root = constructParseTree();
    if (root) {
        generateProgramCode(root);
        delete root;
    }
    return 0;
}
