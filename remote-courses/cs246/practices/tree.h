export module tree;

struct Node {
    int val;
    int numChildren;
    Node** children;

    Node();
    ~Node();
    void print();
};
