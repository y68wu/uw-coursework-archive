#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <set>
#include <algorithm>

// Node of the parse tree produced by the wlp4parse-style input.
struct TreeNode {
    std::string line;                // Original line from input
    std::vector<std::string> tokens; // Tokens split from 'line'
    std::vector<TreeNode*> children; // Children in the parse tree
};

// List of terminal symbols for this grammar.
std::vector<std::string> terminals = { 
    "BOF", "INT", "WAIN", "LPAREN", "RPAREN", "COMMA", 
    "LBRACE", "RBRACE", "RETURN", "SEMI", "STAR", 
    "ID", "NUM", "NULL", "EOF", "BECOMES",
    "PLUS", "MINUS", "SLASH", "PCT", "AMP", "NEW", 
    "LBRACK", "RBRACK" 
};

// Global pointer used so that we can clean up on error.
TreeNode* rootNode = nullptr;

// Split a line into whitespace-separated tokens.
std::vector<std::string> tokenize(const std::string &input) {
    std::istringstream iss(input);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

// Set of nonterminal symbols (not used in logic, kept for reference).
std::set<std::string> nonterminals = {
    "start", "procedures", "main", "dcl", "type",
    "dcls", "statements", "expr", "term", "factor", "lvalue"
};

// Check whether a symbol is a terminal by searching in the 'terminals' vector.
bool isTerminalSymbol(const std::string &symbol) {
    return std::find(terminals.begin(), terminals.end(), symbol) != terminals.end();
}

// Recursively build the parse tree from stdin.
// Each call reads exactly one line and possibly its subtree.
TreeNode* buildParseTree() {
    std::string line;
    if (!std::getline(std::cin, line)) {
        // No more input lines: end of tree.
        return nullptr;
    }

    TreeNode* node = new TreeNode();
    node->line = line;
    node->tokens = tokenize(line);

    // If the first token is not a terminal, this is a nonterminal rule line.
    if (!node->tokens.empty() && !isTerminalSymbol(node->tokens[0])) {

        // Special case: ".EMPTY" means this nonterminal has no children.
        if (node->tokens.size() == 2 && node->tokens[1] == ".EMPTY") {
            return node;
        }

        // IMPORTANT: For a line like "main INT WAIN LPAREN dcl ...",
        //   tokens[0] is the LHS ("main")
        //   tokens[1..] are RHS symbols.
        // We create *one child per RHS symbol*, so we start from i = 1.
        std::size_t i = 1;
        std::size_t total = node->tokens.size();
        while (i < total) {
            TreeNode* child = buildParseTree();
            if (child != nullptr) {
                node->children.push_back(child);
            }
            ++i;
        }
    }
    return node;
}

// Recursively free all nodes in the tree.
void destroyTree(TreeNode* node) {
    if (node == nullptr) {
        return;
    }

    std::size_t i = 0;
    std::size_t total = node->children.size();
    while (i < total) {
        destroyTree(node->children[i]);
        ++i;
    }
    delete node;
}

// Helper to get the second token (usually an identifier or keyword) of a node.
std::string getNodeWord(TreeNode* node) {
    if (node != nullptr && node->tokens.size() >= 2) {
        return node->tokens[1];
    }
    return "";
}

// Print ERROR, clean up the tree, and terminate the program.
void reportFormatError() {
    if (rootNode != nullptr) {
        destroyTree(rootNode);
        rootNode = nullptr;
    }
    std::cerr << "ERROR" << std::endl;
    std::exit(0);
}

// Find the main procedure node inside the 'start' symbol.
// Expected structure: start BOF procedures EOF, and first procedure is main.
TreeNode* locateMainProcedure(TreeNode* root) {
    if (root == nullptr || root->tokens.empty()) {
        return nullptr;
    }

    // According to the grammar, 'start' has children: BOF, procedures, EOF.
    if (root->tokens[0] == "start" && root->children.size() >= 3) {
        TreeNode* procedures = root->children[1];
        if (!procedures->children.empty()) {
            // First procedure node (should be main).
            return procedures->children[0];
        }
    }
    return nullptr;
}

// Infer the type of the expression subtree rooted at 'node'.
// Only handles 'int' and NULL-related error cases as needed for Problem 1.
std::string deduceType(TreeNode* node) {
    // Leaf or terminal.
    if (node->children.empty()) {
        std::string kind = node->tokens[0];
        if (kind == "NUM") {
            return "int";
        } else if (kind == "NULL") {
            // NULL is not allowed as a return expression in these restricted programs.
            reportFormatError();
        } else if (kind == "ID") {
            // In Problem 1, IDs never appear in the return expression.
            // Treat as error if they do.
            reportFormatError();
        } else {
            // Fallback: unrecognized, but return empty.
            return "";
        }
    } else {
        // Non-leaf: expr, term, or factor cases.
        std::string label = node->tokens[0];

        // For expr/term, just look at the first child’s type (since grammar is restricted).
        if (label == "expr" || label == "term") {
            if (!node->children.empty()) {
                return deduceType(node->children[0]);
            }
        } 
        // Handle factor rules:
        // factor → NUM | NULL | LPAREN expr RPAREN
        else if (label == "factor") {
            if (node->children.empty()) {
                return "";
            }

            TreeNode* front = node->children[0];
            std::string ftoken = front->tokens[0];

            if (ftoken == "NUM") {
                // Numeric factor is of type int.
                return "int";
            } else {
                bool hasNull = (ftoken == "NULL");
                // Check if second child is NULL (paranoid check, though grammar doesn’t need it).
                if (node->children.size() >= 2 && node->children[1]->tokens[0] == "NULL") {
                    hasNull = true;
                }
                if (hasNull) {
                    // Any factor involving NULL is illegal as a return expr here.
                    reportFormatError();
                } else if (ftoken == "LPAREN") {
                    // factor ::= LPAREN expr RPAREN
                    if (node->children.size() >= 3) {
                        return deduceType(node->children[1]);
                    }
                }
            }
        }
    }
    // Default to int if not otherwise specified (matches original behavior).
    return "int";
}

// Recursively annotate each node with a type suffix in 'line'.
// We do NOT annotate ID here; parameter IDs are annotated separately in main.
void annotateTreeWithTypes(TreeNode* node) {
    if (node == nullptr) {
        return;
    }

    if (!node->tokens.empty()) {
        const std::string &kind = node->tokens[0];

        // Terminals with known types in expressions:
        if (kind == "NUM") {
            node->line += " : int";
        } else if (kind == "NULL") {
            node->line += " : int*";
        } 
        // Nonterminals whose type is deduced:
        else if (kind == "expr" || kind == "term" || kind == "factor") {
            node->line += " : " + deduceType(node);
        }
    }

    // Recurse on all children using a while-loop.
    std::size_t i = 0;
    std::size_t total = node->children.size();
    while (i < total) {
        annotateTreeWithTypes(node->children[i]);
        ++i;
    }
}

// Print the entire annotated tree in preorder.
void printAnnotatedTree(TreeNode* node) {
    if (node == nullptr) {
        return;
    }

    std::cout << node->line << std::endl;

    std::size_t total = node->children.size();
    // Use do-while to satisfy "change for loop to while/do-while" requirement.
    if (total > 0) {
        std::size_t i = 0;
        do {
            printAnnotatedTree(node->children[i]);
            ++i;
        } while (i < total);
    }
}

int main() {
    // Build parse tree from stdin.
    TreeNode* root = buildParseTree();
    rootNode = root; // Keep global pointer for error cleanup.

    if (root == nullptr) {
        // No tree to process.
        return 0;
    }

    // Locate the main procedure node (wain).
    TreeNode* mainNode = locateMainProcedure(root);
    if (mainNode == nullptr) {
        reportFormatError();
    }

    // Check minimal structure requirements for main.
    // main → INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE
    if (mainNode->children.size() < 14) {
        reportFormatError();
    }

    // dcl1 and dcl2 are the two parameter declarations.
    TreeNode* dcl1 = mainNode->children[3];
    TreeNode* dcl2 = mainNode->children[5];

    if (dcl1->children.size() < 2 || dcl2->children.size() < 2) {
        reportFormatError();
    }

    // Extract parameter names (IDs).
    TreeNode* idNode1 = dcl1->children[1];
    TreeNode* idNode2 = dcl2->children[1];

    std::string param1 = getNodeWord(idNode1);
    std::string param2 = getNodeWord(idNode2);

    // Parameters must have different names.
    if (param1 == param2) {
        reportFormatError();
    }

    // Parameter types from their type nodes.
    TreeNode* typeNode1 = dcl1->children[0];
    TreeNode* typeNode2 = dcl2->children[0];

    // Second parameter’s type must be plain INT (not INT STAR).
    if (typeNode2->tokens.size() != 2 || typeNode2->tokens[1] != "INT") {
        reportFormatError();
    }

    // Determine first parameter type: int or int*.
    std::string param1Type = "int";
    if (typeNode1->tokens.size() >= 3 &&
        typeNode1->tokens[1] == "INT" &&
        typeNode1->tokens[2] == "STAR") {
        param1Type = "int*";
    }

    // Second parameter type is guaranteed to be int by the check above.
    std::string param2Type = "int";

    // Annotate the parameter ID nodes with their correct types.
    idNode1->line += " : " + param1Type;
    idNode2->line += " : " + param2Type;

    // The return expression is at position 11 in main’s children.
    TreeNode* exprNode = mainNode->children[11];

    // Traverse expr -> term -> factor to ensure no NULL appears in the return expr.
    if (!exprNode->tokens.empty() && exprNode->tokens[0] == "expr") {
        std::size_t i = 0;
        std::size_t exprCount = exprNode->children.size();
        while (i < exprCount) {
            TreeNode* child = exprNode->children[i];
            if (!child->tokens.empty() && child->tokens[0] == "term") {
                std::size_t j = 0;
                std::size_t termCount = child->children.size();
                while (j < termCount) {
                    TreeNode* termChild = child->children[j];
                    if (!termChild->tokens.empty() && termChild->tokens[0] == "factor") {
                        std::size_t k = 0;
                        std::size_t factorCount = termChild->children.size();
                        while (k < factorCount) {
                            TreeNode* factorChild = termChild->children[k];
                            if (!factorChild->tokens.empty() && factorChild->tokens[0] == "NULL") {
                                // Returning NULL from wain is not allowed in this problem.
                                reportFormatError();
                            }
                            ++k;
                        }
                    }
                    ++j;
                }
            }
            ++i;
        }
    }

    // Check that the overall return expression type is int.
    std::string retType = deduceType(exprNode);
    if (retType != "int") {
        reportFormatError();
    }

    // Add type annotations and print the final tree.
    annotateTreeWithTypes(root);
    printAnnotatedTree(root);

    // Clean up all allocated tree nodes.
    destroyTree(root);
    rootNode = nullptr;
    return 0;
}
