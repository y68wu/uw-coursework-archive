#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cctype>

// One node of the parse tree, together with optional type info.
struct TreeItem {
    std::vector<std::string> fields;   // grammar line tokens
    std::vector<TreeItem*>   subtrees; // children in the parse tree
    bool                     hasType;  // has this node been typed?
    std::string              typeLabel;// "int" or "int*"
    TreeItem() : hasType(false), typeLabel("") {}
};

// All parsed input lines (each line already tokenized)
std::vector<std::vector<std::string> > globalLines;

// Index of the next line to consume while rebuilding the parse tree
int globalLineIndex = 0;

// Root of the full program tree (used to clean up on error)
TreeItem* globalRoot = nullptr;

// Type tags used for semantic checking
enum ValueKind { VK_INT, VK_PTR, VK_BAD };

// One entry in the custom symbol table
struct SymbolRecord {
    std::string name;
    ValueKind   kind;
};

// Symbol table for variables in wain (parameters + locals)
std::vector<SymbolRecord> symbolStorage;

// Exception used to unwind when a semantic error is detected
struct TypeFailure {};

// Forward declarations for mutual recursion
ValueKind inferExpressionType(TreeItem* node);
ValueKind inferLvalueType(TreeItem* node);

// Break a raw input line into whitespace-separated tokens
std::vector<std::string> tokenizeLine(const std::string &line) {
    std::vector<std::string> pieces;
    std::istringstream reader(line);
    std::string item;
    while (reader >> item) {
        pieces.push_back(item);
    }
    return pieces;
}

// Recognize whether a grammar symbol is a terminal (starts with uppercase)
bool isGrammarTerminal(const std::string &symbol) {
    if (symbol.empty()) return false;
    unsigned char ch = static_cast<unsigned char>(symbol[0]);
    return std::isupper(ch) != 0;
}

// Rebuild the parse tree for a single production rule
TreeItem* buildParseTree() {
    TreeItem* current = new TreeItem();
    current->fields = globalLines[globalLineIndex];
    globalLineIndex++;

    std::size_t pos = 1;
    std::size_t endPos = current->fields.size();
    while (pos < endPos) {
        std::string rhsSymbol = current->fields[pos];

        if (rhsSymbol != ".EMPTY") {
            if (isGrammarTerminal(rhsSymbol)) {
                // Next input line holds the leaf for this terminal
                TreeItem* leaf = new TreeItem();
                leaf->fields = globalLines[globalLineIndex];
                globalLineIndex++;
                current->subtrees.push_back(leaf);
            } else {
                // Nonterminal: recursively read its subtree
                current->subtrees.push_back(buildParseTree());
            }
        }
        ++pos;
    }
    return current;
}

// Recursively destroy an entire subtree
void destroyTree(TreeItem* node) {
    if (!node) return;
    std::size_t idx = 0;
    std::size_t total = node->subtrees.size();
    while (idx < total) {
        destroyTree(node->subtrees[idx]);
        ++idx;
    }
    delete node;
}

// Report a semantic error, clean up the tree, and throw an exception
void triggerError() {
    if (globalRoot) {
        destroyTree(globalRoot);
        globalRoot = nullptr;
    }
    std::cerr << "ERROR" << std::endl;
    throw TypeFailure();
}

// Summary of a declaration (type + identifier name)
struct DeclSummary {
    ValueKind   kind;
    std::string name;
};

// Interpret a dcl node ("type ID") and annotate the ID node with its type
DeclSummary analyzeDeclaration(TreeItem* dclNode) {
    TreeItem* typeNode = dclNode->subtrees[0];
    TreeItem* idNode   = dclNode->subtrees[1];

    ValueKind declaredKind;
    if (typeNode->fields.size() == 2) {
        declaredKind = VK_INT;          // type -> INT
    } else {
        declaredKind = VK_PTR;          // type -> INT STAR
    }

    std::string identName = idNode->fields[1];
    idNode->hasType = true;
    idNode->typeLabel = (declaredKind == VK_INT ? "int" : "int*");

    DeclSummary summary;
    summary.kind = declaredKind;
    summary.name = identName;
    return summary;
}

// Clear all entries from the symbol table
void clearSymbols() {
    symbolStorage.clear();
}

// Count how many entries exist with a given name (0 or 1)
int countSymbol(const std::string &name) {
    std::size_t i = 0;
    std::size_t n = symbolStorage.size();
    while (i < n) {
        if (symbolStorage[i].name == name) {
            return 1;
        }
        ++i;
    }
    return 0;
}

// Insert or overwrite a symbol in the table
void storeSymbol(const std::string &name, ValueKind kind) {
    std::size_t i = 0;
    std::size_t n = symbolStorage.size();
    while (i < n) {
        if (symbolStorage[i].name == name) {
            symbolStorage[i].kind = kind;
            return;
        }
        ++i;
    }
    SymbolRecord rec;
    rec.name = name;
    rec.kind = kind;
    symbolStorage.push_back(rec);
}

// Look up a variable's type; if missing, it is a semantic error
ValueKind lookupVariableType(const std::string &name) {
    std::size_t i = 0;
    std::size_t n = symbolStorage.size();
    while (i < n) {
        if (symbolStorage[i].name == name) {
            return symbolStorage[i].kind;
        }
        ++i;
    }
    triggerError();
    return VK_BAD;
}

// Recursively process local declarations "dcls" and fill the symbol table
void processDeclarations(TreeItem* dclsNode) {
    // Base form: dcls .EMPTY
    if (dclsNode->fields.size() == 2 && dclsNode->fields[1] == ".EMPTY") {
        return;
    }

    // Recursive form: dcls dcl BECOMES NUM/NULL SEMI
    TreeItem* prevDcls = dclsNode->subtrees[0];
    TreeItem* dclNode  = dclsNode->subtrees[1];
    TreeItem* initNode = dclsNode->subtrees[3];

    processDeclarations(prevDcls);

    DeclSummary summary = analyzeDeclaration(dclNode);
    ValueKind declaredKind = summary.kind;
    std::string varName = summary.name;

    // No duplicate local or parameter names
    if (countSymbol(varName) != 0) {
        triggerError();
    }

    // Infer the type of the initializer literal
    ValueKind initKind = VK_BAD;
    if (initNode->fields[0] == "NUM") {
        initKind = VK_INT;
        initNode->hasType = true;
        initNode->typeLabel = "int";
    } else if (initNode->fields[0] == "NULL") {
        initKind = VK_PTR;
        initNode->hasType = true;
        initNode->typeLabel = "int*";
    } else {
        triggerError();
    }

    if (declaredKind != initKind) {
        triggerError();
    }

    storeSymbol(varName, declaredKind);
}

// Type an lvalue node and annotate it
ValueKind inferLvalueType(TreeItem* node) {
    std::size_t sz = node->fields.size();

    // Parenthesized lvalue: (lvalue)
    if (sz == 4 && node->fields[1] == "LPAREN") {
        TreeItem* innerL = node->subtrees[1];
        ValueKind innerKind = inferLvalueType(innerL);
        std::string label = (innerKind == VK_INT ? "int" : "int*");
        node->hasType = true;
        node->typeLabel = label;
        return innerKind;
    }
    // Dereference: *factor
    else if (sz == 3 && node->fields[1] == "STAR") {
        TreeItem* factorNode = node->subtrees[1];
        ValueKind factorKind = inferExpressionType(factorNode);
        if (factorKind != VK_PTR) {
            triggerError();
        }
        node->hasType = true;
        node->typeLabel = "int";
        return VK_INT;
    }
    // Simple identifier
    else if (sz == 2 && node->fields[1] == "ID") {
        TreeItem* idLeaf = node->subtrees[0];
        std::string varName = idLeaf->fields[1];
        ValueKind kind = lookupVariableType(varName);
        std::string label = (kind == VK_INT ? "int" : "int*");
        idLeaf->hasType = true;
        idLeaf->typeLabel = label;
        node->hasType = true;
        node->typeLabel = label;
        return kind;
    }

    // Anything else is not a valid lvalue production
    triggerError();
    return VK_BAD;
}

// Type an expr / term / factor tree and propagate annotations upwards
ValueKind inferExpressionType(TreeItem* node) {
    std::string lhs = node->fields[0];

    // Expression rules
    if (lhs == "expr") {
        std::size_t sz = node->fields.size();

        // Binary arithmetic / pointer arithmetic first
        if (sz == 4) {
            TreeItem* leftExpr  = node->subtrees[0];
            TreeItem* rightTerm = node->subtrees[2];
            std::string opName  = node->fields[2];

            ValueKind kLeft  = inferExpressionType(leftExpr);
            ValueKind kRight = inferExpressionType(rightTerm);

            ValueKind result = VK_BAD;
            if (opName == "PLUS") {
                if (kLeft == VK_INT && kRight == VK_INT) {
                    result = VK_INT;
                } else if ((kLeft == VK_PTR && kRight == VK_INT) ||
                           (kLeft == VK_INT && kRight == VK_PTR)) {
                    result = VK_PTR;
                } else {
                    triggerError();
                }
            } else if (opName == "MINUS") {
                if (kLeft == VK_INT && kRight == VK_INT) {
                    result = VK_INT;
                } else if (kLeft == VK_PTR && kRight == VK_INT) {
                    result = VK_PTR;
                } else if (kLeft == VK_PTR && kRight == VK_PTR) {
                    result = VK_INT;
                } else {
                    triggerError();
                }
            } else {
                triggerError();
            }

            std::string label = (result == VK_INT ? "int" : "int*");
            node->hasType = true;
            node->typeLabel = label;
            return result;
        }
        // Simple expr -> term
        else if (sz == 2) {
            ValueKind kind = inferExpressionType(node->subtrees[0]);
            std::string label = (kind == VK_INT ? "int" : "int*");
            node->hasType = true;
            node->typeLabel = label;
            return kind;
        }

        // No other shapes for expr
        triggerError();
    }

    // Term rules
    if (lhs == "term") {
        std::size_t sz = node->fields.size();

        // term -> term * / % / / factor
        if (sz == 4) {
            TreeItem* leftTerm  = node->subtrees[0];
            TreeItem* rightFact = node->subtrees[2];

            ValueKind kLeft  = inferExpressionType(leftTerm);
            ValueKind kRight = inferExpressionType(rightFact);

            // All term binary operations require integer operands
            if (kLeft != VK_INT || kRight != VK_INT) {
                triggerError();
            }

            node->hasType = true;
            node->typeLabel = "int";
            return VK_INT;
        }
        // term -> factor
        else if (sz == 2) {
            ValueKind kind = inferExpressionType(node->subtrees[0]);
            std::string label = (kind == VK_INT ? "int" : "int*");
            node->hasType = true;
            node->typeLabel = label;
            return kind;
        }

        triggerError();
    }

    // Factor rules
    if (lhs == "factor") {
        std::size_t sz = node->fields.size();

        // factor -> NEW INT LBRACK expr RBRACK
        if (sz == 6 && node->fields[1] == "NEW") {
            TreeItem* sizeExpr = node->subtrees[3];
            ValueKind sizeKind = inferExpressionType(sizeExpr);
            if (sizeKind != VK_INT) {
                triggerError();
            }
            node->hasType = true;
            node->typeLabel = "int*";
            return VK_PTR;
        }
        // factor -> AMP lvalue
        else if (sz == 3 && node->fields[1] == "AMP") {
            TreeItem* lvNode = node->subtrees[1];
            ValueKind baseKind = inferLvalueType(lvNode);
            if (baseKind != VK_INT) {
                triggerError();
            }
            node->hasType = true;
            node->typeLabel = "int*";
            return VK_PTR;
        }
        // factor -> STAR factor   (pointer dereference)
        else if (sz == 3 && node->fields[1] == "STAR") {
            TreeItem* factNode = node->subtrees[1];
            ValueKind baseKind = inferExpressionType(factNode);
            if (baseKind != VK_PTR) {
                triggerError();
            }
            node->hasType = true;
            node->typeLabel = "int";
            return VK_INT;
        }
        // factor -> LPAREN expr RPAREN
        else if (sz == 4 && node->fields[1] == "LPAREN") {
            TreeItem* innerExpr = node->subtrees[1];
            ValueKind kind = inferExpressionType(innerExpr);
            std::string label = (kind == VK_INT ? "int" : "int*");
            node->hasType = true;
            node->typeLabel = label;
            return kind;
        }
        // factor -> ID
        else if (sz == 2 && node->fields[1] == "ID") {
            TreeItem* leaf = node->subtrees[0];
            std::string varName = leaf->fields[1];
            ValueKind kind = lookupVariableType(varName);
            std::string label = (kind == VK_INT ? "int" : "int*");
            leaf->hasType = true;
            leaf->typeLabel = label;
            node->hasType = true;
            node->typeLabel = label;
            return kind;
        }
        // factor -> NUM
        else if (sz == 2 && node->fields[1] == "NUM") {
            TreeItem* leaf = node->subtrees[0];
            leaf->hasType = true;
            leaf->typeLabel = "int";
            node->hasType = true;
            node->typeLabel = "int";
            return VK_INT;
        }
        // factor -> NULL
        else if (sz == 2 && node->fields[1] == "NULL") {
            TreeItem* leaf = node->subtrees[0];
            leaf->hasType = true;
            leaf->typeLabel = "int*";
            node->hasType = true;
            node->typeLabel = "int*";
            return VK_PTR;
        }

        triggerError();
    }

    // lvalue appearing where an expression is expected
    if (lhs == "lvalue") {
        return inferLvalueType(node);
    }

    triggerError();
    return VK_BAD;
}

// Print the annotated parse tree in preorder
void printAnnotatedTree(TreeItem* node) {
    std::size_t idx = 0;
    std::size_t len = node->fields.size();
    while (idx < len) {
        if (idx != 0) {
            std::cout << ' ';
        }
        std::cout << node->fields[idx];
        ++idx;
    }
    if (node->hasType) {
        std::cout << " : " << node->typeLabel;
    }
    std::cout << "\n";

    std::size_t childIdx = 0;
    std::size_t childCount = node->subtrees.size();
    while (childIdx < childCount) {
        printAnnotatedTree(node->subtrees[childIdx]);
        ++childIdx;
    }
}

int main() {
    try {
        // Read and tokenize all lines from stdin
        std::string line;
        while (std::getline(std::cin, line)) {
            globalLines.push_back(tokenizeLine(line));
        }

        if (globalLines.empty()) {
            triggerError();
        }

        // Rebuild parse tree from wlp4parse-style lines
        globalLineIndex = 0;
        globalRoot = buildParseTree();

        // Navigate to main: start -> BOF procedures EOF, procedures -> main
        TreeItem* proceduresNode = globalRoot->subtrees[1];
        TreeItem* mainNode       = proceduresNode->subtrees[0];

        // main -> INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE
        TreeItem* firstDcl   = mainNode->subtrees[3];
        TreeItem* secondDcl  = mainNode->subtrees[5];
        TreeItem* dclsNode   = mainNode->subtrees[8];
        TreeItem* returnExpr = mainNode->subtrees[11];

        clearSymbols();

        // Handle the two parameters
        DeclSummary firstInfo  = analyzeDeclaration(firstDcl);
        DeclSummary secondInfo = analyzeDeclaration(secondDcl);

        storeSymbol(firstInfo.name, firstInfo.kind);
        if (countSymbol(secondInfo.name) != 0) {
            triggerError();
        }
        storeSymbol(secondInfo.name, secondInfo.kind);

        // Second parameter must be int
        if (secondInfo.kind != VK_INT) {
            triggerError();
        }

        // Process local declarations in the body of wain
        processDeclarations(dclsNode);

        // Type-check the return expression of wain
        ValueKind mainReturn = inferExpressionType(returnExpr);
        if (mainReturn != VK_INT) {
            triggerError();
        }

        // If we get here, the program is well-typed; print annotated tree
        printAnnotatedTree(globalRoot);
        destroyTree(globalRoot);
        globalRoot = nullptr;
    } catch (TypeFailure &) {
        // Error already printed by triggerError()
    }

    return 0;
}
