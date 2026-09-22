#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <utility>

// This struct represents a single node in the parse tree produced by wlp4parse.
// Each node stores the original line, its tokenized form, and pointers to child nodes.
struct Node {
    std::string rawLine;                     // Raw line from the wlp4parse output
    std::vector<std::string> symbolList;     // Individual tokens split from 'rawLine'
    std::vector<Node*> childNodes;           // Child nodes in the parse tree
    std::string nodeType;                    // Optional type info (not used in this generator)

    ~Node() {
        // Recursively delete all child nodes using a while-loop
        std::size_t childIndex = 0;
        while (childIndex < childNodes.size()) {
            delete childNodes[childIndex];
            ++childIndex;
        }
    }
};

// This struct acts as a lightweight "map" entry: it associates a variable name
// with a stack offset (relative to register $29).
struct VarEntry {
    std::string varIdentifier;   // Variable identifier
    int stackOffset;             // Stack offset from $29 for that identifier
};

// Global vector used as a simple symbol table for variables.
// We avoid <map> by storing (name, offset) pairs and searching linearly.
std::vector<VarEntry> gSymbolTable;

// Global vector storing all terminal symbols from the WLP4 grammar
std::vector<std::string> gTerminals;

// Global offset used for allocating new local variables on the stack.
// It starts at -8 (after storing the two wain parameters) and decreases by 4
// for each new local variable.
int gNextStackOffset = 0;

// Global counter used for generating unique labels for if/else/while code.
int gNextLabelId = 0;

// Populate the list of terminal tokens for the grammar.
// This lets us distinguish between leaf and non-leaf parse-tree nodes.
void setupTerminalList() {
    const char* terminalNameArray[] = {
        "BOF", "INT", "WAIN", "LPAREN", "RPAREN", "COMMA",
        "LBRACE", "RBRACE", "RETURN", "SEMI", "STAR",
        "ID", "NUM", "NULL", "EOF", "BECOMES",
        "PLUS", "MINUS", "SLASH", "PCT",
        "IF", "ELSE", "WHILE", "EQ", "NE", "LT", "GT", "LE", "GE",
        "PUTCHAR", "PRINTLN", "GETCHAR"
    };

    std::size_t terminalCount = sizeof(terminalNameArray) / sizeof(terminalNameArray[0]);
    std::size_t terminalIndex = 0;
    while (terminalIndex < terminalCount) {
        gTerminals.push_back(std::string(terminalNameArray[terminalIndex]));
        ++terminalIndex;
    }
}

// Check if a given symbol name is a terminal by scanning the 'gTerminals' vector.
bool checkTerminalSymbol(const std::string &symbolToken) {
    std::size_t terminalPos = 0;
    while (terminalPos < gTerminals.size()) {
        if (gTerminals[terminalPos] == symbolToken) {
            return true;
        }
        ++terminalPos;
    }
    return false;
}

// Search for a variable in 'gSymbolTable' by name.
// Returns true and sets 'foundOffset' if found, otherwise returns false.
bool lookupVariableOffset(const std::string &searchName, int &foundOffset) {
    std::size_t varIndex = 0;
    while (varIndex < gSymbolTable.size()) {
        if (gSymbolTable[varIndex].varIdentifier == searchName) {
            foundOffset = gSymbolTable[varIndex].stackOffset;
            return true;
        }
        ++varIndex;
    }
    return false;
}

// Insert a new variable name and offset into 'gSymbolTable' if it is not already present.
void registerVariableSymbol(const std::string &newName, int newOffset) {
    int discardOffset = 0;
    if (!lookupVariableOffset(newName, discardOffset)) {
        VarEntry tableEntry;
        tableEntry.varIdentifier = newName;
        tableEntry.stackOffset = newOffset;
        gSymbolTable.push_back(tableEntry);
    }
}

// Generate a fresh label name by concatenating a prefix with a unique integer.
std::string generateUniqueLabel(const std::string &labelBase) {
    std::string finalLabel = labelBase;
    finalLabel += std::to_string(gNextLabelId);
    ++gNextLabelId;
    return finalLabel;
}

// Recursively build the parse tree by reading lines from stdin.
// This function is equivalent in behaviour to the original 'parse_tree'.
Node* constructParseTree() {
    std::string parseLine;
    if (!std::getline(std::cin, parseLine)) {
        return nullptr;
    }

    Node* currentNode = new Node();
    currentNode->rawLine = parseLine;

    // Split the line into tokens using a string stream.
    std::istringstream parseStream(parseLine);
    std::string currentToken;
    while (parseStream >> currentToken) {
        currentNode->symbolList.push_back(currentToken);
    }

    // If this node has no tokens, something is malformed; treat it as leaf.
    if (currentNode->symbolList.empty()) {
        return currentNode;
    }

    // Leaf node if the first token is a terminal or the production is explicitly ".EMPTY".
    if (checkTerminalSymbol(currentNode->symbolList[0]) ||
        (currentNode->symbolList.size() == 2 && currentNode->symbolList[1] == ".EMPTY")) {
        return currentNode;
    }

    // Otherwise, create child nodes for each non-":" token (the grammar format uses ":")
    std::size_t childTokenIndex = 1;
    while (childTokenIndex < currentNode->symbolList.size()) {
        if (currentNode->symbolList[childTokenIndex] == ":" ||
            (childTokenIndex > 0 && currentNode->symbolList[childTokenIndex - 1] == ":")) {
            ++childTokenIndex;
            continue;
        }
        Node* childNode = constructParseTree();
        if (childNode != nullptr) {
            currentNode->childNodes.push_back(childNode);
        }
        ++childTokenIndex;
    }

    return currentNode;
}

// Extract type and identifier name from a "dcl" node following grammar: dcl -> type ID.
// For this assignment, the underlying type always reduces to "int".
std::pair<std::string, std::string> parseDeclarationNode(Node* declNode) {
    std::string declTypeStr = "int";        // The wlp4type assignment ensures final type is int
    std::string declVarName;

    if (declNode != nullptr && declNode->childNodes.size() > 1) {
        Node* identNode = declNode->childNodes[1];
        if (identNode != nullptr && identNode->symbolList.size() > 1) {
            declVarName = identNode->symbolList[1];
        }
    }

    return std::pair<std::string, std::string>(declTypeStr, declVarName);
}

// Helper retained from the original code to locate a "value" node inside nested factors.
// It is not essential for code generation here but is kept to preserve behaviour.
Node* findInnerValueNode(Node* exprNode) {
    if (exprNode == nullptr) {
        return nullptr;
    }

    if (exprNode->symbolList.size() > 0 && exprNode->symbolList[0] == "factor" &&
        exprNode->childNodes.size() == 3 &&
        exprNode->childNodes[0]->symbolList.size() > 0 &&
        exprNode->childNodes[0]->symbolList[0] == "LPAREN") {
        return findInnerValueNode(exprNode->childNodes[1]);
    }

    if (exprNode->symbolList.size() > 0 && exprNode->symbolList[0] == "factor" &&
        exprNode->childNodes.size() == 1) {
        return exprNode->childNodes[0];
    }

    if (!exprNode->childNodes.empty()) {
        return findInnerValueNode(exprNode->childNodes[0]);
    }

    return nullptr;
}

// Forward declarations so that the mutually recursive code-generation functions compile.
void generateExpressionCode(Node* astNode);
void generateStatementListCode(Node* stmtListNode);

// Recursively handle declaration lists (dcls) and allocate stack space for locals.
// This function is equivalent in effect to the original 'process_dcls'.
void generateDeclarationCode(Node* declsNode) {
    if (declsNode == nullptr) {
        return;
    }

    // Base case: no declarations or explicit empty production
    if (declsNode->childNodes.empty() ||
        (declsNode->symbolList.size() > 1 && declsNode->symbolList[1] == ".EMPTY")) {
        return;
    }

    // Recursive case for grammar: dcls -> dcls dcl BECOMES NUM SEMI
    if (declsNode->childNodes.size() == 5) {
        generateDeclarationCode(declsNode->childNodes[0]);

        Node* singleDeclNode = declsNode->childNodes[1];
        Node* literalNode = declsNode->childNodes[3];

        if (singleDeclNode != nullptr && singleDeclNode->childNodes.size() == 2) {
            std::pair<std::string, std::string> declInfo = parseDeclarationNode(singleDeclNode);
            std::string varName = declInfo.second;

            if (!varName.empty()) {
                // Match the original logic: symbolTable[name] = nextOffset; nextOffset -= 4;
                int newStackOffset = gNextStackOffset;
                registerVariableSymbol(varName, newStackOffset);
                gNextStackOffset -= 4;

                // Initialize variable with 0 or with the literal value
                if (literalNode->symbolList.size() > 1 && literalNode->symbolList[1] == "0") {
                    std::cout << "sw $0, " << newStackOffset << "($29)" << std::endl;
                } else {
                    std::cout << "lis $3" << std::endl;
                    std::cout << ".word " << literalNode->symbolList[1] << std::endl;
                    std::cout << "sw $3, " << newStackOffset << "($29)" << std::endl;
                }

                // Reserve space on the stack for this new local
                std::cout << "sub $30, $30, $4" << std::endl;
            }
        }
    }
}

// Emit code for arithmetic expressions, leaving the final result in register $3.
// This function covers expr, term, factor, and lvalue nonterminals.
void generateExpressionCode(Node* astNode) {
    if (astNode == nullptr || astNode->symbolList.empty()) {
        return;
    }

    std::string nodeLabel = astNode->symbolList[0];

    if (nodeLabel == "expr") {
        if (astNode->childNodes.size() == 1) {
            // expr -> term
            generateExpressionCode(astNode->childNodes[0]);
        } else if (astNode->childNodes.size() == 3 &&
                   astNode->childNodes[0]->symbolList.size() > 0 &&
                   astNode->childNodes[0]->symbolList[0] == "expr") {
            // expr -> expr PLUS|MINUS term
            generateExpressionCode(astNode->childNodes[0]);

            // Save left operand on stack
            std::cout << "sw $3, -4($30)" << std::endl;
            std::cout << "sub $30, $30, $4" << std::endl;

            // Evaluate right operand
            generateExpressionCode(astNode->childNodes[2]);

            // Restore left operand into $5
            std::cout << "add $30, $30, $4" << std::endl;
            std::cout << "lw $5, -4($30)" << std::endl;

            if (astNode->childNodes[1]->symbolList[0] == "PLUS") {
                std::cout << "add $3, $5, $3" << std::endl;
            } else if (astNode->childNodes[1]->symbolList[0] == "MINUS") {
                std::cout << "sub $3, $5, $3" << std::endl;
            }
        }
    } else if (nodeLabel == "term") {
        if (astNode->childNodes.size() == 1) {
            // term -> factor
            generateExpressionCode(astNode->childNodes[0]);
        } else if (astNode->childNodes.size() == 3 &&
                   astNode->childNodes[0]->symbolList.size() > 0 &&
                   astNode->childNodes[0]->symbolList[0] == "term") {
            // term -> term STAR|SLASH|PCT factor
            generateExpressionCode(astNode->childNodes[0]);

            // Save left operand in $3
            std::cout << "sw $3, -4($30)" << std::endl;
            std::cout << "sub $30, $30, $4" << std::endl;

            // Evaluate right operand
            generateExpressionCode(astNode->childNodes[2]);

            // Restore left operand into $5
            std::cout << "add $30, $30, $4" << std::endl;
            std::cout << "lw $5, -4($30)" << std::endl;

            if (astNode->childNodes[1]->symbolList[0] == "STAR") {
                std::cout << "mult $5, $3" << std::endl;
                std::cout << "mflo $3" << std::endl;
            } else if (astNode->childNodes[1]->symbolList[0] == "SLASH") {
                std::cout << "div $5, $3" << std::endl;
                std::cout << "mflo $3" << std::endl;
            } else if (astNode->childNodes[1]->symbolList[0] == "PCT") {
                std::cout << "div $5, $3" << std::endl;
                std::cout << "mfhi $3" << std::endl;
            }
        }
    } else if (nodeLabel == "factor") {
        // Handle constants, variable references, GETCHAR, and parenthesized expressions
        if (astNode->childNodes.size() == 1) {
            Node* factorChild = astNode->childNodes[0];
            if (factorChild->symbolList[0] == "ID") {
                // factor -> ID
                std::string loadVarName = factorChild->symbolList[1];
                int loadOffset = 0;
                if (lookupVariableOffset(loadVarName, loadOffset)) {
                    std::cout << "lw $3, " << loadOffset << "($29)" << std::endl;
                }
            } else if (factorChild->symbolList[0] == "NUM") {
                // factor -> NUM
                std::cout << "lis $3" << std::endl;
                std::cout << ".word " << factorChild->symbolList[1] << std::endl;
            }
        } else if (astNode->childNodes.size() == 3 &&
                   astNode->childNodes[0]->symbolList[0] == "GETCHAR") {
            // factor -> GETCHAR LPAREN RPAREN
            std::cout << "lis $5" << std::endl;
            std::cout << ".word 0xffff0004" << std::endl;
            std::cout << "lw $3, 0($5)" << std::endl;
        } else if (astNode->childNodes.size() == 3 &&
                   astNode->childNodes[0]->symbolList[0] == "LPAREN" &&
                   astNode->childNodes[2]->symbolList[0] == "RPAREN") {
            // factor -> LPAREN expr RPAREN
            generateExpressionCode(astNode->childNodes[1]);
        }
    } else if (nodeLabel == "lvalue") {
        // Evaluate lvalue into $3 by loading the variable from its saved address
        if (astNode->childNodes.size() == 1 &&
            astNode->childNodes[0]->symbolList[0] == "ID") {
            std::string loadVarName = astNode->childNodes[0]->symbolList[1];
            int loadOffset = 0;
            if (lookupVariableOffset(loadVarName, loadOffset)) {
                std::cout << "lw $3, " << loadOffset << "($29)" << std::endl;
            }
        } else if (astNode->childNodes.size() == 3 &&
                   astNode->childNodes[0]->symbolList[0] == "LPAREN" &&
                   astNode->childNodes[2]->symbolList[0] == "RPAREN") {
            // lvalue -> LPAREN lvalue RPAREN
            generateExpressionCode(astNode->childNodes[1]);
        }
    }
}

// Emit comparison logic for a 'test' nonterminal and branch to true/false labels.
// The logic mirrors the original code for EQ, NE, LT, LE, GT, and GE.
void generateTestBranchCode(Node* testAstNode,
                            const std::string &labelTrue,
                            const std::string &labelFalse) {
    if (testAstNode == nullptr) {
        return;
    }

    // Evaluate left side into $3
    generateExpressionCode(testAstNode->childNodes[0]);
    std::cout << "sw $3, -4($30)" << std::endl;
    std::cout << "sub $30, $30, $4" << std::endl;

    // Evaluate right side into $3
    generateExpressionCode(testAstNode->childNodes[2]);
    std::cout << "add $30, $30, $4" << std::endl;
    std::cout << "lw $5, -4($30)" << std::endl;

    std::string compareOp = testAstNode->childNodes[1]->symbolList[0];
    if (compareOp == "EQ") {
        std::cout << "beq $5, $3, " << labelTrue << std::endl;
        std::cout << "beq $0, $0, " << labelFalse << std::endl;
    } else if (compareOp == "NE") {
        std::cout << "bne $5, $3, " << labelTrue << std::endl;
        std::cout << "beq $0, $0, " << labelFalse << std::endl;
    } else if (compareOp == "LT") {
        std::cout << "slt $3, $5, $3" << std::endl;
        std::cout << "bne $3, $0, " << labelTrue << std::endl;
        std::cout << "beq $0, $0, " << labelFalse << std::endl;
    } else if (compareOp == "LE") {
        std::cout << "slt $3, $3, $5" << std::endl;
        std::cout << "beq $3, $0, " << labelTrue << std::endl;
        std::cout << "beq $0, $0, " << labelFalse << std::endl;
    } else if (compareOp == "GT") {
        std::cout << "slt $3, $3, $5" << std::endl;
        std::cout << "bne $3, $0, " << labelTrue << std::endl;
        std::cout << "beq $0, $0, " << labelFalse << std::endl;
    } else if (compareOp == "GE") {
        std::cout << "slt $3, $5, $3" << std::endl;
        std::cout << "beq $3, $0, " << labelTrue << std::endl;
        std::cout << "beq $0, $0, " << labelFalse << std::endl;
    }
}

// Recursively determine the variable name from an lvalue subtree.
// This handles both "lvalue -> ID" and "lvalue -> LPAREN lvalue RPAREN".
std::string extractLvalueIdentifier(Node* lvalueAstNode) {
    if (lvalueAstNode == nullptr) {
        return std::string();
    }

    if (lvalueAstNode->childNodes.size() == 1 &&
        lvalueAstNode->childNodes[0]->symbolList.size() > 1) {
        return lvalueAstNode->childNodes[0]->symbolList[1];
    } else if (lvalueAstNode->childNodes.size() == 3) {
        return extractLvalueIdentifier(lvalueAstNode->childNodes[1]);
    }

    return std::string();
}

// Forward declaration to allow mutual recursion between statement-list and single-statement emitters.
void generateSingleStatementCode(Node* stmtNode);

// Emit code for a list of statements under the 'statements' nonterminal.
// This respects left recursion in the grammar: statements -> statements statement.
void generateStatementListCode(Node* stmtListNode) {
    if (stmtListNode == nullptr || stmtListNode->childNodes.empty()) {
        return;
    }

    if (stmtListNode->childNodes.size() == 2) {
        generateStatementListCode(stmtListNode->childNodes[0]);
        generateSingleStatementCode(stmtListNode->childNodes[1]);
    }
}

// Emit code for one individual statement (assignment, if/else, while, PUTCHAR, or PRINTLN).
// This function mirrors the behaviour of your original 'gen_statement'.
void generateSingleStatementCode(Node* stmtNode) {
    if (stmtNode == nullptr) {
        return;
    }

    // 1) Assignment form: statement -> lvalue BECOMES expr SEMI
    if (stmtNode->childNodes.size() == 4) {
        generateExpressionCode(stmtNode->childNodes[2]);
        std::string targetName = extractLvalueIdentifier(stmtNode->childNodes[0]);

        if (targetName == "$1") {
            std::cout << "sw $3, -0($29)" << std::endl;
        } else if (targetName == "$2") {
            std::cout << "sw $3, -4($29)" << std::endl;
        } else {
            int storeOffset = 0;
            if (lookupVariableOffset(targetName, storeOffset)) {
                std::cout << "sw $3, " << storeOffset << "($29)" << std::endl;
            }
        }
    }
    // 2) If-else form:
    //    statement -> IF LPAREN test RPAREN LBRACE statements RBRACE
    //                 ELSE LBRACE statements RBRACE
    else if (stmtNode->childNodes.size() == 11) {
        std::string labelIf = generateUniqueLabel("if");
        std::string labelElse = generateUniqueLabel("else");
        std::string labelEndIf = generateUniqueLabel("end");

        generateTestBranchCode(stmtNode->childNodes[2], labelIf, labelElse);

        std::cout << labelIf << ":" << std::endl;
        generateStatementListCode(stmtNode->childNodes[5]);
        std::cout << "beq $0, $0, " << labelEndIf << std::endl;

        std::cout << labelElse << ":" << std::endl;
        generateStatementListCode(stmtNode->childNodes[9]);
        std::cout << labelEndIf << ":" << std::endl;
    }
    // 3) While loop form:
    //    statement -> WHILE LPAREN test RPAREN LBRACE statements RBRACE
    else if (stmtNode->childNodes.size() == 7) {
        std::string labelWhileCond = generateUniqueLabel("while");
        std::string labelWhileBody = generateUniqueLabel("body");
        std::string labelWhileExit = generateUniqueLabel("whileend");

        std::cout << labelWhileCond << ":" << std::endl;
        generateTestBranchCode(stmtNode->childNodes[2], labelWhileBody, labelWhileExit);

        std::cout << labelWhileBody << ":" << std::endl;
        generateStatementListCode(stmtNode->childNodes[5]);
        std::cout << "beq $0, $0, " << labelWhileCond << std::endl;
        std::cout << labelWhileExit << ":" << std::endl;
    }
    // 4) I/O forms:
    //    statement -> PUTCHAR LPAREN expr RPAREN SEMI
    //    statement -> PRINTLN LPAREN expr RPAREN SEMI
    else if (stmtNode->childNodes.size() == 5) {
        generateExpressionCode(stmtNode->childNodes[2]);

        // Fixed PUTCHAR: directly write the character in $3 to the console MMIO
        if (stmtNode->childNodes[0]->symbolList[0] == "PUTCHAR") {
            std::cout << "lis $5" << std::endl;
            std::cout << ".word 0xffff000c" << std::endl;
            std::cout << "sw $3, 0($5)" << std::endl;
        } else if (stmtNode->childNodes[0]->symbolList[0] == "PRINTLN") {
            // Preserve $3, $1, and $31 around a call to the imported 'print' routine.
            std::cout << "sw $3, -4($30)" << std::endl;
            std::cout << "sub $30, $30, $4" << std::endl;
            std::cout << "sw $1, -4($30)" << std::endl;
            std::cout << "sub $30, $30, $4" << std::endl;

            std::cout << "add $1, $3, $0" << std::endl;

            std::cout << "sw $31, -4($30)" << std::endl;
            std::cout << "sub $30, $30, $4" << std::endl;

            std::cout << "lis $5" << std::endl;
            std::cout << ".word print" << std::endl;
            std::cout << "jalr $5" << std::endl;

            std::cout << "add $30, $30, $4" << std::endl;
            std::cout << "lw $31, -4($30)" << std::endl;
            std::cout << "add $30, $30, $4" << std::endl;
            std::cout << "lw $1, -4($30)" << std::endl;
            std::cout << "add $30, $30, $4" << std::endl;
            std::cout << "lw $3, -4($30)" << std::endl;
        }
    }
}

// Emit the entire WLP4 program in MIPS code, starting from the root parse-tree node.
// This function is equivalent to your original 'generate_code'.
void generateProgramCode(Node* rootNode) {
    if (rootNode == nullptr || rootNode->childNodes.size() < 2) {
        return;
    }

    Node* proceduresNode = rootNode->childNodes[1];
    Node* mainNode = proceduresNode->childNodes[0];

    // According to the grammar:
    // mainNode children: 0:INT 1:WAIN 2:LPAREN 3:dcl 4:COMMA 5:dcl 6:RPAREN
    //                     7:LBRACE 8:dcls 9:statements 10:RETURN 11:expr 12:SEMI 13:RBRACE
    Node* firstParamNode = mainNode->childNodes[3];
    Node* secondParamNode = mainNode->childNodes[5];
    Node* declsNode = mainNode->childNodes[8];
    Node* stmtsNode = mainNode->childNodes[9];
    Node* returnNode = mainNode->childNodes[11];

    std::cout << ".import print" << std::endl;

    // Set up a constant 4 in $4 and position $29 one word below $30.
    // This establishes a frame pointer that will not move for locals.
    std::cout << "lis $4" << std::endl;
    std::cout << ".word 4" << std::endl;
    std::cout << "sub $29, $30, $4" << std::endl;

    // Extract names of the two wain parameters.
    std::pair<std::string, std::string> param1Info = parseDeclarationNode(firstParamNode);
    std::pair<std::string, std::string> param2Info = parseDeclarationNode(secondParamNode);
    std::string param1VarName = param1Info.second;
    std::string param2VarName = param2Info.second;

    // Save parameter registers $1 and $2 at fixed slots in the stack frame.
    std::cout << "sw $1, 0($29)" << std::endl;
    std::cout << "sub $30, $30, $4" << std::endl;
    std::cout << "sw $2, -4($29)" << std::endl;
    std::cout << "sub $30, $30, $4" << std::endl;

    // Remember these parameter locations in our variable table.
    registerVariableSymbol(param1VarName, 0);
    registerVariableSymbol(param2VarName, -4);
    gNextStackOffset = -8;

    // Emit code for declarations and statements in the body of wain.
    if (declsNode != nullptr) {
        generateDeclarationCode(declsNode);
    }
    if (stmtsNode != nullptr) {
        generateStatementListCode(stmtsNode);
    }
    if (returnNode != nullptr) {
        generateExpressionCode(returnNode);
    }

    // Compute how many local variables were allocated and deallocate that space
    // by moving the stack pointer $30 back up.
    int localVarCount = (-gNextStackOffset) / 4;
    int frameIndex = 0;
    while (frameIndex < localVarCount) {
        std::cout << "add $30, $30, $4" << std::endl;
        ++frameIndex;
    }

    // Return from wain to the caller (the runtime).
    std::cout << "jr $31" << std::endl;
}

int main() {
    // First, set up the list of terminals for recognizing leaf nodes.
    setupTerminalList();

    // Then read the entire parse tree from standard input.
    Node* rootNode = constructParseTree();
    if (rootNode != nullptr) {
        // Generate MIPS code for the program.
        generateProgramCode(rootNode);
        delete rootNode;
    }

    return 0;
}
