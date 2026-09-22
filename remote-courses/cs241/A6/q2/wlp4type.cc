#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cctype>

// One node in the parse tree plus optional type annotation.
struct TreeNode {
    std::vector<std::string> labels;  // tokens for this grammar line
    std::vector<TreeNode*>   kids;    // children in the parse tree
    bool                     typed;   // whether this node already has a type
    std::string              typeTag; // "int" or "int*"
    TreeNode() : typed(false), typeTag("") {}
};

// All lines of wlp4parse output, already tokenized.
std::vector<std::vector<std::string> > parseLines;
// Current index while reconstructing the tree.
int parseIndex = 0;

// Types that can appear in this restricted subset.
enum TypeKind { TK_INT, TK_PTR, TK_BAD };

// One variable in the single wain scope.
struct ScopeEntry {
    std::string name;
    TypeKind    kind;
};

// Simple linear “symbol table”.
std::vector<ScopeEntry> scopeTable;


// Turn one input line into a vector of tokens.
std::vector<std::string> tokenizeInputLine(const std::string &raw) {
    std::vector<std::string> out;
    std::istringstream reader(raw);
    std::string piece;
    while (reader >> piece) {
        out.push_back(piece);
    }
    return out;
}

// Decide if a grammar symbol is terminal (starts with uppercase).
bool isGrammarTerminal(const std::string &symbol) {
    if (symbol.empty()) return false;
    unsigned char ch = static_cast<unsigned char>(symbol[0]);
    return std::isupper(ch) != 0;
}

// Rebuild one subtree from parseLines starting at parseIndex.
TreeNode* buildTreeFromLines() {
    TreeNode* node = new TreeNode();
    node->labels = parseLines[parseIndex];
    parseIndex++;

    std::size_t pos = 1;
    std::size_t limit = node->labels.size();
    while (pos < limit) {
        std::string rhsSym = node->labels[pos];

        if (rhsSym != ".EMPTY") {
            if (isGrammarTerminal(rhsSym)) {
                // A terminal symbol: the next line in the input is a leaf.
                TreeNode* leaf = new TreeNode();
                leaf->labels = parseLines[parseIndex];
                parseIndex++;
                node->kids.push_back(leaf);
            } else {
                // A nonterminal: recursively consume its production.
                node->kids.push_back(buildTreeFromLines());
            }
        }
        ++pos;
    }
    return node;
}

// Summary of a single declaration (its type and identifier).
struct DeclSummary {
    TypeKind    kind;
    std::string var;
};

// Interpret a dcl node as "type ID" and annotate the ID child.
DeclSummary extractDeclInfo(TreeNode* dclNode) {
    TreeNode* typeNode = dclNode->kids[0];
    TreeNode* idNode   = dclNode->kids[1];

    TypeKind declKind;
    if (typeNode->labels.size() == 2) {
        declKind = TK_INT;       // type -> INT
    } else {
        declKind = TK_PTR;       // type -> INT STAR
    }

    std::string ident = idNode->labels[1];
    idNode->typed = true;
    idNode->typeTag = (declKind == TK_INT ? "int" : "int*");

    DeclSummary info;
    info.kind = declKind;
    info.var  = ident;
    return info;
}

// Drop all entries from the scope table.
void clearScopeTable() {
    scopeTable.clear();
}

// Test whether a name already appears in the scope table.
bool scopeContains(const std::string &name) {
    std::size_t i = 0;
    std::size_t n = scopeTable.size();
    while (i < n) {
        if (scopeTable[i].name == name) {
            return true;
        }
        ++i;
    }
    return false;
}

// Add a new variable and its type into the scope table.
void addScopeEntry(const std::string &name, TypeKind kind) {
    ScopeEntry entry;
    entry.name = name;
    entry.kind = kind;
    scopeTable.push_back(entry);
}

// Look up a name's type; sets found flag to indicate success.
TypeKind lookupScopeType(const std::string &name, bool &found) {
    std::size_t i = 0;
    std::size_t n = scopeTable.size();
    while (i < n) {
        if (scopeTable[i].name == name) {
            found = true;
            return scopeTable[i].kind;
        }
        ++i;
    }
    found = false;
    return TK_BAD;
}

// Infer and annotate the type of an expression subtree.
TypeKind inferNodeType(TreeNode* sub) {
    std::string head = sub->labels[0];

    // Only one production for expr in this problem.
    if (head == "expr") {
        TypeKind t = inferNodeType(sub->kids[0]);
        if (t == TK_BAD) return TK_BAD;
        sub->typed = true;
        sub->typeTag = (t == TK_INT ? "int" : "int*");
        return t;
    }

    // term also just forwards to its child.
    if (head == "term") {
        TypeKind t = inferNodeType(sub->kids[0]);
        if (t == TK_BAD) return TK_BAD;
        sub->typed = true;
        sub->typeTag = (t == TK_INT ? "int" : "int*");
        return t;
    }

    if (head == "factor") {
        std::size_t sz = sub->labels.size();

        // Parenthesized expression: ( expr )
        if (sz == 4 && sub->labels[1] == "LPAREN") {
            TypeKind inner = inferNodeType(sub->kids[1]);
            if (inner == TK_BAD) return TK_BAD;
            sub->typed = true;
            sub->typeTag = (inner == TK_INT ? "int" : "int*");
            return inner;
        }
        // Variable reference: ID
        if (sz == 2 && sub->labels[1] == "ID") {
            TreeNode* idLeaf = sub->kids[0];
            std::string name = idLeaf->labels[1];
            bool found = false;
            TypeKind t = lookupScopeType(name, found);
            if (!found) {
                // Undeclared identifier in the expression.
                return TK_BAD;
            }
            idLeaf->typed = true;
            idLeaf->typeTag = (t == TK_INT ? "int" : "int*");
            sub->typed = true;
            sub->typeTag = idLeaf->typeTag;
            return t;
        }
        // Numeric literal.
        if (sz == 2 && sub->labels[1] == "NUM") {
            TreeNode* numLeaf = sub->kids[0];
            numLeaf->typed = true;
            numLeaf->typeTag = "int";
            sub->typed = true;
            sub->typeTag = "int";
            return TK_INT;
        }
        // NULL literal.
        if (sz == 2 && sub->labels[1] == "NULL") {
            TreeNode* nullLeaf = sub->kids[0];
            nullLeaf->typed = true;
            nullLeaf->typeTag = "int*";
            sub->typed = true;
            sub->typeTag = "int*";
            return TK_PTR;
        }

        // Any other shape is not valid in this restricted grammar.
        return TK_BAD;
    }

    // Non-expression nodes are ignored by this helper.
    return TK_BAD;
}

// Print the annotated tree in preorder.
void printAnnotatedTree(TreeNode* sub) {
    std::size_t pos = 0;
    std::size_t len = sub->labels.size();
    while (pos < len) {
        if (pos != 0) {
            std::cout << ' ';
        }
        std::cout << sub->labels[pos];
        ++pos;
    }
    if (sub->typed) {
        std::cout << " : " << sub->typeTag;
    }
    std::cout << "\n";

    std::size_t idx = 0;
    std::size_t totalKids = sub->kids.size();
    while (idx < totalKids) {
        printAnnotatedTree(sub->kids[idx]);
        ++idx;
    }
}

// Recursively free the parse tree to avoid leaks.
void freeTree(TreeNode* node) {
    if (!node) return;
    std::size_t idx = 0;
    std::size_t cnt = node->kids.size();
    while (idx < cnt) {
        freeTree(node->kids[idx]);
        ++idx;
    }
    delete node;
}

// Build symbol table entries for local declarations in dcls.
void processDcls(TreeNode* dclsNode, bool &okFlag) {
    // Empty block: dcls .EMPTY
    if (dclsNode->labels.size() == 2 && dclsNode->labels[1] == ".EMPTY") return;
    if (dclsNode->kids.empty()) return;

    // Recursive form: dcls dcl BECOMES NUM/NULL SEMI
    TreeNode* prevNode = dclsNode->kids[0];
    TreeNode* dclNode  = dclsNode->kids[1];
    TreeNode* initNode = dclsNode->kids[3];

    processDcls(prevNode, okFlag);
    if (!okFlag) return;

    DeclSummary info = extractDeclInfo(dclNode);
    TypeKind varKind = info.kind;
    std::string varName = info.var;

    if (scopeContains(varName)) {
        okFlag = false;
        return;
    }

    TypeKind initKind;
    if (initNode->labels[0] == "NUM") {
        initKind = TK_INT;
        initNode->typed = true;
        initNode->typeTag = "int";
    } else if (initNode->labels[0] == "NULL") {
        initKind = TK_PTR;
        initNode->typed = true;
        initNode->typeTag = "int*";
    } else {
        okFlag = false;
        return;
    }

    if (varKind != initKind) {
        okFlag = false;
        return;
    }

    addScopeEntry(varName, varKind);
}

int main() {
    std::string inputLine;
    while (std::getline(std::cin, inputLine)) {
        if (!inputLine.empty()) {
            parseLines.push_back(tokenizeInputLine(inputLine));
        }
    }

    if (parseLines.empty()) {
        std::cerr << "ERROR" << std::endl;
        return 0;
    }

    parseIndex = 0;
    TreeNode* programNode = buildTreeFromLines();

    bool isValid = true;

    // start -> BOF procedures EOF
    if (programNode->kids.size() < 3) {
        isValid = false;
    } else {
        TreeNode* procsNode = programNode->kids[1];
        if (procsNode->kids.empty()) {
            isValid = false;
        } else {
            // procedures -> main
            TreeNode* wainNode = procsNode->kids[0];
            // main -> INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE
            if (wainNode->kids.size() < 14) {
                isValid = false;
            } else {
                TreeNode* param1Node   = wainNode->kids[3];
                TreeNode* param2Node   = wainNode->kids[5];
                TreeNode* bodyDclsNode = wainNode->kids[8];
                TreeNode* returnNode   = wainNode->kids[11];

                DeclSummary p1 = extractDeclInfo(param1Node);
                DeclSummary p2 = extractDeclInfo(param2Node);

                // Parameter names must be distinct.
                if (p1.var == p2.var) {
                    isValid = false;
                }
                // Second parameter must be plain int.
                if (p2.kind != TK_INT) {
                    isValid = false;
                }

                clearScopeTable();
                addScopeEntry(p1.var, p1.kind);
                addScopeEntry(p2.var, p2.kind);

                if (isValid) {
                    processDcls(bodyDclsNode, isValid);
                }

                if (isValid) {
                    TypeKind retKind = inferNodeType(returnNode);
                    if (retKind != TK_INT) {
                        isValid = false;
                    }
                }
            }
        }
    }

    if (!isValid) {
        freeTree(programNode);
        std::cerr << "ERROR" << std::endl;
        return 0;
    }

    printAnnotatedTree(programNode);
    freeTree(programNode);
    return 0;
}
