#include "tree.h"
#include "iostream"

using namespace std;

Node::Node() {
    cin >> val >> numChildren;
    if (numChildren == 0) {
        children = nullptr;
    } else {
        children = new Node*[numChildren];
        for (int i = 0; i < numChildren; i++) {
            children[i] = new Node;
        }
    }
}

Node::~Node() {
    for (int i = 0; i < numChildren; i++) {
        delete children[i];
    }
}

void Node::print() {
    for (int i = 0; i < numChildren; i++) {
        children[i]->print();
    }
    cout << val << numChildren << endl;
}
