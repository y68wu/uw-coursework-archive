#include <iostream>
#include <sstream>
#include <string>

// Small dynamic array helper without vector
template <typename T>
struct SimpleArray {
    T *data;
    int size;
    int capacity;

    SimpleArray() : data(0), size(0), capacity(0) {}

    // Deep copy constructor so each array owns its own storage
    SimpleArray(const SimpleArray &other) : data(0), size(0), capacity(0) {
        if (other.size > 0) {
            capacity = other.capacity;
            data = new T[capacity];
            size = other.size;
            for (int i = 0; i < size; ++i) {
                data[i] = other.data[i];
            }
        }
    }

    // Deep copy assignment so arrays do not share buffers
    SimpleArray &operator=(const SimpleArray &other) {
        if (this != &other) {
            delete[] data;
            data = 0;
            size = 0;
            capacity = 0;
            if (other.size > 0) {
                capacity = other.capacity;
                data = new T[capacity];
                size = other.size;
                for (int i = 0; i < size; ++i) {
                    data[i] = other.data[i];
                }
            }
        }
        return *this;
    }

    // Destructor frees the storage of this array
    ~SimpleArray() {
        delete[] data;
    }

    int getSize() const { return size; }

    bool empty() const { return size == 0; }

    void clear() {
        size = 0;
    }

    void push_back(const T &value) {
        if (size == capacity) {
            int newCap = (capacity == 0 ? 4 : capacity * 2);
            T *newData = new T[newCap];
            for (int i = 0; i < size; ++i) {
                newData[i] = data[i];
            }
            delete[] data;
            data = newData;
            capacity = newCap;
        }
        data[size] = value;
        ++size;
    }

    T &operator[](int index) {
        return data[index];
    }

    const T &operator[](int index) const {
        return data[index];
    }
};

// Check if a token is a terminal symbol in the grammar
bool isTerminal(const std::string &token) {
    return token == "INT"      || token == "WAIN"    || token == "LPAREN"  ||
           token == "COMMA"    || token == "RPAREN"  || token == "LBRACE"  ||
           token == "RETURN"   || token == "SEMI"    || token == "RBRACE"  ||
           token == "BOF"      || token == "EOF"     || token == "ID"      ||
           token == "NUM"      || token == "NULL"    || token == "BECOMES" ||
           token == "IF"       || token == "ELSE"    || token == "WHILE"   ||
           token == "PRINTLN"  || token == "PUTCHAR" || token == "GETCHAR" ||
           token == "EQ"       || token == "NE"      || token == "LT"      ||
           token == "GT"       || token == "LE"      || token == "GE"      ||
           token == "PLUS"     || token == "MINUS"   || token == "STAR"    ||
           token == "SLASH"    || token == "PCT"     ||
           token == "NEW"      || token == "DELETE"  ||
           token == "LBRACK"   || token == "RBRACK"  || token == "AMP";
}

// Entry in one symbol table
// id is variable name
// type is int or int*
struct SymbolEntry {
    std::string id;
    std::string type;
    int offset;

    SymbolEntry() : id(), type(), offset(0) {}

    SymbolEntry(const std::string &identifier,
                const std::string &kind,
                int off)
        : id(identifier), type(kind), offset(off) {}
};

// Node in the parse tree
// value is the nonterminal or terminal name
// type is from wlp4type output
struct ParseNode {
    std::string value;
    SimpleArray<ParseNode *> children;
    std::string type;

    explicit ParseNode(const std::string &val)
        : value(val), children(), type() {}

    ~ParseNode() {
        int idx = 0;
        int n = children.getSize();
        while (idx < n) {
            delete children[idx];
            ++idx;
        }
    }

    void populate();
};

// Symbol table for one procedure
struct ProcedureSymbolTable {
    std::string name;
    SimpleArray<SymbolEntry> symbols;

    ProcedureSymbolTable() : name(), symbols() {}
};

// Parameter type info for one procedure
struct ProcedureParamInfo {
    std::string name;
    SimpleArray<std::string> paramTypes;

    ProcedureParamInfo() : name(), paramTypes() {}
};

// Build subtree for this node from wlp4ti style input
void ParseNode::populate() {
    std::string line;
    if (!std::getline(std::cin, line)) {
        return;
    }
    std::istringstream iss(line);

    std::string token;
    std::string currentSymbol;
    iss >> currentSymbol;

    bool isEmptyChildren = false;
    std::size_t colonPos = line.find(':');

    if (colonPos != std::string::npos) {
        std::string left = line.substr(0, colonPos);
        std::istringstream leftStream(left);
        leftStream >> token;

        while (leftStream >> token) {
            if (token == ".EMPTY") {
                isEmptyChildren = true;
                break;
            }
            ParseNode *child = new ParseNode(token);
            children.push_back(child);
        }

        std::string right = line.substr(colonPos + 1);
        std::istringstream rightStream(right);
        rightStream >> token;
        type = token;

        if (isTerminal(currentSymbol) || isEmptyChildren) {
            return;
        }

        int idx = 0;
        int n = children.getSize();
        while (idx < n) {
            children[idx]->populate();
            ++idx;
        }
    } else {
        bool skipRemainder = false;

        while (iss >> token) {
            if (token == ".EMPTY") {
                isEmptyChildren = true;
            }
            if (token == ":") {
                skipRemainder = true;
                continue;
            }
            if (!skipRemainder) {
                ParseNode *child = new ParseNode(token);
                children.push_back(child);
            }
        }

        if (isTerminal(currentSymbol) || isEmptyChildren) {
            return;
        }

        int idx = 0;
        int n = children.getSize();
        while (idx < n) {
            children[idx]->populate();
            ++idx;
        }
    }
}

// Code generator for WLP4 to MIPS
class CodeGenerator {
public:
    bool hasError;
    std::string output;

    int deleteLabelCounter;
    int ifLabelCounter;
    int whileLabelCounter;

    ParseNode *root;
    bool wainHasArrayParam;
    std::string activeProcedure;

    SimpleArray<ProcedureSymbolTable> symbolTables;
    SimpleArray<ProcedureParamInfo> procedureParams;

    CodeGenerator()
        : hasError(false),
          output(),
          deleteLabelCounter(0),
          ifLabelCounter(0),
          whileLabelCounter(0),
          root(0),
          wainHasArrayParam(false),
          activeProcedure(),
          symbolTables(),
          procedureParams() {}

    ~CodeGenerator() {
        delete root;
    }

    void constructTree();
    void performGeneration() { generateFromNode(root); }

    bool encounteredError() const { return hasError; }
    std::string getOutput() const { return output; }

    void generateFromNode(ParseNode *current);

    void emitWain(ParseNode *current);
    void emitProcedure(ParseNode *current);

    void handleDeclaration(ParseNode *current);
    void handleDeclarations(ParseNode *current);
    void handleStatements(ParseNode *current);
    void handleSingleStatement(ParseNode *current);

    void handleExpression(ParseNode *current);
    void handleTerm(ParseNode *current);
    void handleFactor(ParseNode *current);
    void handleLValue(ParseNode *current, int &offset);
    void handleTest(ParseNode *current);

    void handleArgList(ParseNode *current, int &argCount);
    void handleParams(ParseNode *current,
                      SimpleArray<std::string> &paramTypes,
                      SimpleArray<std::string> &paramNames);
    void handleParamList(ParseNode *current,
                         SimpleArray<std::string> &paramTypes,
                         SimpleArray<std::string> &paramNames);

    ProcedureSymbolTable *findSymbolTable(const std::string &procName) {
        int i = 0;
        int n = symbolTables.getSize();
        while (i < n) {
            if (symbolTables[i].name == procName) {
                return &symbolTables[i];
            }
            ++i;
        }
        return 0;
    }

    ProcedureSymbolTable *getOrCreateSymbolTable(const std::string &procName) {
        ProcedureSymbolTable *existing = findSymbolTable(procName);
        if (existing != 0) {
            return existing;
        }
        ProcedureSymbolTable newEntry;
        newEntry.name = procName;
        symbolTables.push_back(newEntry);
        int idx = symbolTables.getSize() - 1;
        return &symbolTables[idx];
    }

    ProcedureParamInfo *findParamInfo(const std::string &procName) {
        int i = 0;
        int n = procedureParams.getSize();
        while (i < n) {
            if (procedureParams[i].name == procName) {
                return &procedureParams[i];
            }
            ++i;
        }
        return 0;
    }

    bool isProcedureRegistered(const std::string &procName) {
        return (findParamInfo(procName) != 0);
    }

    SymbolEntry *lookupSymbol(const std::string &id);

    void addSymbolToTable(const std::string &id,
                          const std::string &type,
                          int offset);

    void registerProcedure(const std::string &name,
                           const SimpleArray<std::string> &params);

    void emitLoadForSymbol(const SymbolEntry *symbol);
    void emitLoadForNumber(const std::string &numLiteral);

    std::string emitPrologue() const;
    std::string emitPush(const std::string &reg) const;
    std::string emitPop(const std::string &reg) const;
};

// Read the first line and build the root of the parse tree
void CodeGenerator::constructTree() {
    std::string line;
    if (!std::getline(std::cin, line)) {
        return;
    }
    std::istringstream iss(line);

    std::string lhs;
    std::string token;
    iss >> lhs;

    root = new ParseNode(lhs);

    while (iss >> token) {
        ParseNode *child = new ParseNode(token);
        root->children.push_back(child);
    }

    if (isTerminal(lhs)) {
        return;
    }

    int idx = 0;
    int n = root->children.getSize();
    while (idx < n) {
        root->children[idx]->populate();
        ++idx;
    }
}

// Insert a variable into the symbol table of the current procedure
void CodeGenerator::addSymbolToTable(const std::string &id,
                                     const std::string &type,
                                     int offset) {
    ProcedureSymbolTable *tablePtr = getOrCreateSymbolTable(activeProcedure);
    SymbolEntry *existing = lookupSymbol(id);
    if (existing != 0) {
        hasError = true;
    } else {
        SymbolEntry sym(id, type, offset);
        tablePtr->symbols.push_back(sym);
    }
}

// Register parameter types of a procedure
void CodeGenerator::registerProcedure(const std::string &name,
                                      const SimpleArray<std::string> &params) {
    if (isProcedureRegistered(name)) {
        hasError = true;
    } else {
        ProcedureParamInfo info;
        info.name = name;
        int i = 0;
        int n = params.getSize();
        while (i < n) {
            info.paramTypes.push_back(params[i]);
            ++i;
        }
        procedureParams.push_back(info);
    }
}

// Find a variable in the current symbol table
SymbolEntry *CodeGenerator::lookupSymbol(const std::string &id) {
    ProcedureSymbolTable *tablePtr = getOrCreateSymbolTable(activeProcedure);
    SimpleArray<SymbolEntry> &table = tablePtr->symbols;
    int i = 0;
    int n = table.getSize();
    while (i < n) {
        if (table[i].id == id) {
            return &table[i];
        }
        ++i;
    }
    return 0;
}

// Emit imports and unconditional branch to wain
std::string CodeGenerator::emitPrologue() const {
    return std::string(".import print\n"
                       ".import init\n"
                       ".import new\n"
                       ".import delete\n") +
           std::string("beq $0, $0, wain\n");
}

// Emit store and stack pointer update to push a register
std::string CodeGenerator::emitPush(const std::string &reg) const {
    return "sw " + reg + ", -4($30)\n"
           "sub $30, $30, $4\n";
}

// Emit reverse of push to restore a register
std::string CodeGenerator::emitPop(const std::string &reg) const {
    return "add $30, $30, $4\n"
           "lw " + reg + ", -4($30)\n";
}

// Load variable from its offset
void CodeGenerator::emitLoadForSymbol(const SymbolEntry *symbol) {
    output += "lw $3, " + std::to_string(symbol->offset) + "($29)\n";
}

// Load integer literal into register three
void CodeGenerator::emitLoadForNumber(const std::string &numLiteral) {
    output += "lis $3\n.word " + numLiteral + "\n";
}

// Walk the parse tree and generate code
void CodeGenerator::generateFromNode(ParseNode *current) {
    if (current == 0) {
        return;
    }

    if (current->value == "start") {
        output += emitPrologue();
        generateFromNode(current->children[1]);
    } else if (current->value == "procedures") {
        int childCount = current->children.getSize();
        if (childCount == 1) {
            emitWain(current->children[0]);
        } else if (childCount == 2) {
            emitProcedure(current->children[0]);
            generateFromNode(current->children[1]);
        }
    } else if (current->value == "main") {
        emitWain(current);
    } else if (current->value == "dcls") {
        handleDeclarations(current);
    } else if (current->value == "dcl") {
        handleDeclaration(current);
    } else if (current->value == "expr") {
        handleExpression(current);
    } else if (current->value == "term") {
        handleTerm(current);
    } else if (current->value == "statements") {
        handleStatements(current);
    } else if (current->value == "test") {
        handleTest(current);
    } else {
        int idx = 0;
        int n = current->children.getSize();
        while (idx < n) {
            generateFromNode(current->children[idx]);
            ++idx;
        }
    }
}

// Emit code for wain including heap initialization
void CodeGenerator::emitWain(ParseNode *current) {
    if (current == 0) {
        return;
    }

    activeProcedure = "wain";
    ProcedureSymbolTable *symTablePtr = getOrCreateSymbolTable(activeProcedure);
    SimpleArray<SymbolEntry> &symbolTable = symTablePtr->symbols;

    SimpleArray<std::string> parameterTypes;

    ParseNode *firstDecl = current->children[3];
    ParseNode *secondDecl = current->children[5];

    ParseNode *firstTypeNode = firstDecl->children[0];
    ParseNode *firstIdNode = firstDecl->children[1];
    std::string firstId = firstIdNode->children[0]->value;
    std::string firstType = (firstTypeNode->children.getSize() == 1) ? "int" : "int*";
    wainHasArrayParam = (firstType == "int*");
    firstIdNode->type = firstType;
    int currentOffset = 4 * (2 - 0);
    addSymbolToTable(firstId, firstType, currentOffset);

    ParseNode *secondTypeNode = secondDecl->children[0];
    ParseNode *secondIdNode = secondDecl->children[1];
    std::string secondId = secondIdNode->children[0]->value;
    std::string secondType = (secondTypeNode->children.getSize() == 1) ? "int" : "int*";
    secondIdNode->type = secondType;
    currentOffset = 4 * (2 - 1);
    addSymbolToTable(secondId, secondType, currentOffset);

    parameterTypes.push_back(firstType);
    parameterTypes.push_back(secondType);
    registerProcedure("wain", parameterTypes);

    output += "wain:\n";
    output += "lis $4\n.word 4\n";
    output += "lis $11\n.word 1\n";
    output += "lis $10\n.word print\n";
    output += emitPush("$1");
    output += emitPush("$2");
    output += "sub $29, $30, $4\n";

    if (wainHasArrayParam) {
        output += emitPush("$2");
        output += emitPush("$31");
        output += "lis $5\n.word init\njalr $5\n";
        output += emitPop("$31");
        output += emitPop("$2");
    } else {
        output += emitPush("$2");
        output += "add $2, $0, $0\n";
        output += emitPush("$31");
        output += "lis $5\n.word init\njalr $5\n";
        output += emitPop("$31");
        output += emitPop("$2");
    }

    handleDeclarations(current->children[8]);
    handleStatements(current->children[9]);
    handleExpression(current->children[11]);

    ProcedureParamInfo *winfo = findParamInfo("wain");
    int paramCount = 0;
    if (winfo != 0) {
        paramCount = winfo->paramTypes.getSize();
    }
    int totalSymbols = symbolTable.getSize();
    int localsToPop = totalSymbols - paramCount;

    int i = 0;
    while (i < localsToPop) {
        output += "add $30, $30, $4\n";
        ++i;
    }

    output += "jr $31\n";
}

// Emit code for a non wain procedure
void CodeGenerator::emitProcedure(ParseNode *current) {
    std::string procName = current->children[1]->children[0]->value;
    activeProcedure = procName;

    SimpleArray<std::string> paramTypes;
    SimpleArray<std::string> paramNames;

    handleParams(current->children[3], paramTypes, paramNames);
    registerProcedure(procName, paramTypes);

    ProcedureSymbolTable *tablePtr = getOrCreateSymbolTable(procName);
    tablePtr->symbols.clear();

    int paramCount = paramTypes.getSize();
    int idx = 0;
    int paramNamesSize = paramNames.getSize();
    while (idx < paramNamesSize) {
        int offset = 4 * (paramCount - idx);
        addSymbolToTable(paramNames[idx], paramTypes[idx], offset);
        ++idx;
    }

    output += "F" + procName + ":\n";
    output += "sub $29, $30, $4\n";

    handleDeclarations(current->children[6]);
    handleStatements(current->children[7]);

    ProcedureSymbolTable *activeTable = getOrCreateSymbolTable(activeProcedure);
    SimpleArray<SymbolEntry> &symbolTable = activeTable->symbols;

    ProcedureParamInfo *pinfo = findParamInfo(activeProcedure);
    int paramNum = 0;
    if (pinfo != 0) {
        paramNum = pinfo->paramTypes.getSize();
    }
    int localNum = symbolTable.getSize() - paramNum;

    handleExpression(current->children[9]);

    int i = 0;
    while (i < localNum) {
        output += "add $30, $30, $4\n";
        ++i;
    }

    output += "add $30, $29, $4\n";
    output += "jr $31\n";
}

// Read the parameters of a procedure
void CodeGenerator::handleParams(ParseNode *current,
                                 SimpleArray<std::string> &paramTypes,
                                 SimpleArray<std::string> &paramNames) {
    if (current->children[0]->value == ".EMPTY") {
        return;
    }

    if (current->children[0]->value == "paramlist") {
        handleParamList(current->children[0], paramTypes, paramNames);
    }
}

// Handle a paramlist node
void CodeGenerator::handleParamList(ParseNode *current,
                                    SimpleArray<std::string> &paramTypes,
                                    SimpleArray<std::string> &paramNames) {
    int childCount = current->children.getSize();
    if (childCount == 1) {
        ParseNode *declNode = current->children[0];
        ParseNode *typeNode = declNode->children[0];
        ParseNode *idNode = declNode->children[1];

        std::string id = idNode->children[0]->value;
        std::string type = (typeNode->children.getSize() == 1) ? "int" : "int*";
        idNode->type = type;
        paramNames.push_back(id);
        paramTypes.push_back(type);
    } else if (childCount == 3) {
        ParseNode *declNode = current->children[0];
        ParseNode *typeNode = declNode->children[0];
        ParseNode *idNode = declNode->children[1];

        std::string id = idNode->children[0]->value;
        std::string type = (typeNode->children.getSize() == 1) ? "int" : "int*";
        idNode->type = type;
        paramNames.push_back(id);
        paramTypes.push_back(type);

        handleParamList(current->children[2], paramTypes, paramNames);
    } else {
        hasError = true;
    }
}

// Handle one declaration dcl
void CodeGenerator::handleDeclaration(ParseNode *current) {
    if (current == 0) {
        return;
    }

    ProcedureSymbolTable *tablePtr = getOrCreateSymbolTable(activeProcedure);
    SimpleArray<SymbolEntry> &symbolTable = tablePtr->symbols;

    ParseNode *typeNode = current->children[0];
    ParseNode *idNode = current->children[1];
    std::string id = idNode->children[0]->value;

    std::string type = (typeNode->children.getSize() == 1) ? "int" : "int*";
    idNode->type = type;

    int tableSize = symbolTable.getSize();
    ProcedureParamInfo *pinfo = findParamInfo(activeProcedure);
    int paramCount = 0;
    if (pinfo != 0) {
        paramCount = pinfo->paramTypes.getSize();
    }
    int currentOffset = (tableSize - paramCount) * (-4);

    addSymbolToTable(id, type, currentOffset);
}

// Handle possibly recursive dcls
void CodeGenerator::handleDeclarations(ParseNode *current) {
    if (current == 0 || current->children[0]->value == ".EMPTY") {
        return;
    }

    ParseNode *next = current->children[0];
    handleDeclarations(next);

    ParseNode *declNode = current->children[1];
    handleDeclaration(declNode);

    ParseNode *initNode = current->children[3];
    std::string id = declNode->children[1]->children[0]->value;
    SymbolEntry *sym = lookupSymbol(id);
    if (sym == 0) {
        hasError = true;
        return;
    }

    if (initNode->value == "NUM") {
        initNode->type = "int";
        std::string num = initNode->children[0]->value;
        output += "lis $3\n";
        output += ".word " + num + "\n";
        output += emitPush("$3");
    } else if (initNode->value == "NULL") {
        initNode->type = "int*";
        output += "lis $3\n";
        output += ".word 1\n";
        output += emitPush("$3");
    } else {
        hasError = true;
    }
}

// Handle a sequence of statements
void CodeGenerator::handleStatements(ParseNode *current) {
    if (current == 0 || current->children[0]->value == ".EMPTY") {
        return;
    }

    int childCount = current->children.getSize();
    if (childCount == 2) {
        handleStatements(current->children[0]);
        handleSingleStatement(current->children[1]);
    } else {
        hasError = true;
    }
}

// Handle a single statement node
void CodeGenerator::handleSingleStatement(ParseNode *current) {
    if (current == 0) {
        return;
    }

    int childCount = current->children.getSize();

    if (childCount == 4 && current->children[1]->value == "BECOMES") {
        ParseNode *lvNode = current->children[0];
        int offset = 0;
        handleLValue(lvNode, offset);

        output += emitPush("$3");
        ParseNode *exprNode = current->children[2];
        handleExpression(exprNode);
        output += emitPop("$5");
        output += "sw $3, 0($5)\n";

    } else if (current->children[0]->value == "DELETE") {
        deleteLabelCounter++;
        output += emitPush("$1");
        handleExpression(current->children[3]);
        output += "beq $3, $11, skipDelete" +
                  std::to_string(deleteLabelCounter) + "\n";
        output += "add $1, $3, $0\n";
        output += emitPush("$31");
        output += "lis $5\n.word delete\njalr $5\n";
        output += emitPop("$31");
        output += "skipDelete" + std::to_string(deleteLabelCounter) + ":\n";
        output += emitPop("$1");

    } else if (current->children[0]->value == "IF") {
        ifLabelCounter++;
        int currentIf = ifLabelCounter;

        handleTest(current->children[2]);
        output += "beq $3, $0, else" + std::to_string(currentIf) + "\n";
        handleStatements(current->children[5]);
        output += "beq $0, $0, endif" + std::to_string(currentIf) + "\n";
        output += "else" + std::to_string(currentIf) + ":\n";
        handleStatements(current->children[9]);
        output += "endif" + std::to_string(currentIf) + ":\n";

    } else if (current->children[0]->value == "WHILE") {
        whileLabelCounter++;
        int currentWhile = whileLabelCounter;

        output += "while" + std::to_string(currentWhile) + ":\n";
        handleTest(current->children[2]);
        output += "beq $3, $0, endWhile" +
                  std::to_string(currentWhile) + "\n";
        handleStatements(current->children[5]);
        output += "beq $0, $0, while" + std::to_string(currentWhile) + "\n";
        output += "endWhile" + std::to_string(currentWhile) + ":\n";

    } else if (current->children[0]->value == "PUTCHAR") {
        handleExpression(current->children[2]);
        output += "lis $5\n";
        output += ".word 0xffff000c\n";
        output += "sw $3, 0($5)\n";

    } else if (current->children[0]->value == "PRINTLN") {
        output += emitPush("$1");
        handleExpression(current->children[2]);
        output += "add $1, $3, $0\n";
        output += emitPush("$31");
        output += "jalr $10\n";
        output += emitPop("$31");
        output += emitPop("$1");
    } else {
        hasError = true;
    }
}

// Compute address for an lvalue
void CodeGenerator::handleLValue(ParseNode *current, int &offset) {
    if (current == 0) {
        return;
    }

    int childCount = current->children.getSize();
    if (childCount == 1) {
        std::string id = current->children[0]->children[0]->value;
        SymbolEntry *entry = lookupSymbol(id);
        if (entry != 0) {
            offset = entry->offset;
            output += "lis $3\n";
            output += ".word " + std::to_string(offset) + "\n";
            output += "add $3, $3, $29\n";
        } else {
            hasError = true;
        }
    } else if (childCount == 3) {
        handleLValue(current->children[1], offset);
    } else if (current->children[0]->value == "STAR") {
        handleFactor(current->children[1]);
    } else {
        hasError = true;
    }
}

// Handle expr including pointer arithmetic
void CodeGenerator::handleExpression(ParseNode *current) {
    if (current == 0) {
        return;
    }

    int childCount = current->children.getSize();
    if (childCount == 1) {
        if (current->children[0]->value == "term") {
            handleTerm(current->children[0]);
        } else {
            hasError = true;
        }
    } else if (childCount == 3) {
        ParseNode *op = current->children[1];
        ParseNode *lhs = current->children[0];
        ParseNode *rhs = current->children[2];

        if (op->value == "PLUS") {
            if (lhs->type == "int" && rhs->type == "int") {
                handleExpression(lhs);
                output += emitPush("$3");
                handleTerm(rhs);
                output += emitPop("$5");
                output += "add $3, $5, $3\n";
            } else if (lhs->type == "int*" && rhs->type == "int") {
                handleExpression(lhs);
                output += emitPush("$3");
                handleTerm(rhs);
                output += "mult $3, $4\n";
                output += "mflo $3\n";
                output += emitPop("$5");
                output += "add $3, $5, $3\n";
            } else if (lhs->type == "int" && rhs->type == "int*") {
                handleExpression(lhs);
                output += "mult $3, $4\n";
                output += "mflo $3\n";
                output += emitPush("$3");
                handleTerm(rhs);
                output += emitPop("$5");
                output += "add $3, $5, $3\n";
            } else {
                hasError = true;
            }
        } else if (op->value == "MINUS") {
            if (lhs->type == "int" && rhs->type == "int") {
                handleExpression(lhs);
                output += emitPush("$3");
                handleTerm(rhs);
                output += emitPop("$5");
                output += "sub $3, $5, $3\n";
            } else if (lhs->type == "int*" && rhs->type == "int") {
                handleExpression(lhs);
                output += emitPush("$3");
                handleTerm(rhs);
                output += "mult $3, $4\n";
                output += "mflo $3\n";
                output += emitPop("$5");
                output += "sub $3, $5, $3\n";
            } else if (lhs->type == "int*" && rhs->type == "int*") {
                handleExpression(lhs);
                output += emitPush("$3");
                handleTerm(rhs);
                output += emitPop("$5");
                output += "sub $3, $5, $3\n";
                output += "div $3, $4\n";
                output += "mflo $3\n";
            } else {
                hasError = true;
            }
        } else {
            hasError = true;
        }
    } else {
        hasError = true;
    }
}

// Handle term with multiplication division and modulo
void CodeGenerator::handleTerm(ParseNode *current) {
    if (current == 0) {
        return;
    }

    int childCount = current->children.getSize();
    if (childCount == 1) {
        if (current->children[0]->value == "factor") {
            handleFactor(current->children[0]);
        } else {
            hasError = true;
        }
    } else if (childCount == 3) {
        ParseNode *op = current->children[1];

        if (op->value == "STAR") {
            handleTerm(current->children[0]);
            output += emitPush("$3");
            handleFactor(current->children[2]);
            output += emitPop("$5");
            output += "mult $5, $3\n";
            output += "mflo $3\n";
        } else if (op->value == "SLASH") {
            handleTerm(current->children[0]);
            output += emitPush("$3");
            handleFactor(current->children[2]);
            output += emitPop("$5");
            output += "div $5, $3\n";
            output += "mflo $3\n";
        } else if (op->value == "PCT") {
            handleTerm(current->children[0]);
            output += emitPush("$3");
            handleFactor(current->children[2]);
            output += emitPop("$5");
            output += "div $5, $3\n";
            output += "mfhi $3\n";
        } else {
            hasError = true;
        }
    } else {
        hasError = true;
    }
}

// Build argument list on stack for procedure calls
void CodeGenerator::handleArgList(ParseNode *current, int &argCount) {
    int childCount = current->children.getSize();
    if (childCount == 1) {
        ++argCount;
        handleExpression(current->children[0]);
        output += emitPush("$3");
    } else if (childCount == 3) {
        ++argCount;
        handleExpression(current->children[0]);
        output += emitPush("$3");
        handleArgList(current->children[2], argCount);
    }
}

// Handle factor nodes including new call and pointer operators
void CodeGenerator::handleFactor(ParseNode *current) {
    if (current == 0) {
        return;
    }

    ParseNode *child = current->children[0];
    int childCount = current->children.getSize();

    if (child->value == "NEW") {
        output += emitPush("$1");
        handleExpression(current->children[3]);
        output += "add $1, $3, $0\n";
        output += emitPush("$31");
        output += "lis $5\n.word new\njalr $5\n";
        output += emitPop("$31");
        output += "bne $3, $0, 1\n";
        output += "add $3, $11, $0\n";
        output += emitPop("$1");

    } else if (child->value == "NUM") {
        std::string num = child->children[0]->value;
        emitLoadForNumber(num);

    } else if (child->value == "NULL") {
        output += "add $3, $0, $11\n";

    } else if (child->value == "ID" && childCount == 1) {
        std::string id = child->children[0]->value;
        SymbolEntry *entry = lookupSymbol(id);
        if (entry != 0) {
            emitLoadForSymbol(entry);
        } else {
            hasError = true;
        }

    } else if (child->value == "ID" && childCount == 3) {
        std::string id = child->children[0]->value;
        output += emitPush("$29");
        output += emitPush("$31");
        output += "lis $5\n";
        output += ".word F" + id + "\n";
        output += "jalr $5\n";
        output += emitPop("$31");
        output += emitPop("$29");

    } else if (child->value == "ID" && childCount == 4) {
        std::string id = child->children[0]->value;
        int argCount = 0;
        output += emitPush("$29");
        output += emitPush("$31");
        handleArgList(current->children[2], argCount);
        output += "lis $5\n";
        output += ".word F" + id + "\n";
        output += "jalr $5\n";
        int i = 0;
        while (i < argCount) {
            output += "add $30, $30, $4\n";
            ++i;
        }
        output += emitPop("$31");
        output += emitPop("$29");

    } else if (child->value == "AMP") {
        ParseNode *lvNode = current->children[1];
        int offset = 0;
        handleLValue(lvNode, offset);

    } else if (child->value == "STAR") {
        handleFactor(current->children[1]);
        output += "lw $3, 0($3)\n";

    } else if (child->value == "LPAREN") {
        handleExpression(current->children[1]);

    } else if (child->value == "GETCHAR") {
        output += "lis $5\n";
        output += ".word 0xffff0004\n";
        output += "lw $3, 0($5)\n";
    }
}

// Handle test expressions for if and while with pointer and integer cases
void CodeGenerator::handleTest(ParseNode *current) {
    if (current == 0) {
        return;
    }

    ParseNode *op = current->children[1];
    ParseNode *lhs = current->children[0];
    ParseNode *rhs = current->children[2];

    if (lhs->type == "int*") {
        if (op->value == "LT") {
            handleExpression(lhs);
            output += emitPush("$3");
            handleExpression(rhs);
            output += emitPop("$5");
            output += "sltu $3, $5, $3\n";
        } else if (op->value == "GT") {
            handleExpression(lhs);
            output += emitPush("$3");
            handleExpression(rhs);
            output += emitPop("$5");
            output += "sltu $3, $3, $5\n";
        } else if (op->value == "NE") {
            handleExpression(lhs);
            output += emitPush("$3");
            handleExpression(rhs);
            output += emitPop("$5");
            output += "sltu $6, $3, $5\n";
            output += "sltu $7, $5, $3\n";
            output += "add $3, $6, $7\n";
        } else if (op->value == "EQ") {
            handleExpression(lhs);
            output += emitPush("$3");
            handleExpression(rhs);
            output += emitPop("$5");
            output += "sltu $6, $3, $5\n";
            output += "sltu $7, $5, $3\n";
            output += "add $3, $6, $7\n";
            output += "sub $3, $11, $3\n";
        } else if (op->value == "LE") {
            handleExpression(lhs);
            output += emitPush("$3");
            handleExpression(rhs);
            output += emitPop("$5");
            output += "sltu $3, $3, $5\n";
            output += "sub $3, $11, $3\n";
        } else if (op->value == "GE") {
            handleExpression(lhs);
            output += emitPush("$3");
            handleExpression(rhs);
            output += emitPop("$5");
            output += "sltu $3, $5, $3\n";
            output += "sub $3, $11, $3\n";
        } else {
            hasError = true;
        }
    } else if (lhs->type == "int") {
        if (op->value == "LT") {
            handleExpression(lhs);
            output += emitPush("$3");
            handleExpression(rhs);
            output += emitPop("$5");
            output += "slt $3, $5, $3\n";
        } else if (op->value == "GT") {
            handleExpression(lhs);
            output += emitPush("$3");
            handleExpression(rhs);
            output += emitPop("$5");
            output += "slt $3, $3, $5\n";
        } else if (op->value == "NE") {
            handleExpression(lhs);
            output += emitPush("$3");
            handleExpression(rhs);
            output += emitPop("$5");
            output += "slt $6, $3, $5\n";
            output += "slt $7, $5, $3\n";
            output += "add $3, $6, $7\n";
        } else if (op->value == "EQ") {
            handleExpression(lhs);
            output += emitPush("$3");
            handleExpression(rhs);
            output += emitPop("$5");
            output += "slt $6, $3, $5\n";
            output += "slt $7, $5, $3\n";
            output += "add $3, $6, $7\n";
            output += "sub $3, $11, $3\n";
        } else if (op->value == "LE") {
            handleExpression(lhs);
            output += emitPush("$3");
            handleExpression(rhs);
            output += emitPop("$5");
            output += "slt $3, $3, $5\n";
            output += "sub $3, $11, $3\n";
        } else if (op->value == "GE") {
            handleExpression(lhs);
            output += emitPush("$3");
            handleExpression(rhs);
            output += emitPop("$5");
            output += "slt $3, $5, $3\n";
            output += "sub $3, $11, $3\n";
        } else {
            hasError = true;
        }
    }
}

// Print error message for marmoset
void reportError() {
    std::cerr << "ERROR" << std::endl;
}

int main() {
    CodeGenerator generator;
    generator.constructTree();
    generator.performGeneration();

    if (generator.encounteredError()) {
        reportError();
        return 1;
    }

    std::cout << generator.getOutput() << std::endl;
    return 0;
}
