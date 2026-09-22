#include "iostream"
#include "fstream"

using namespace std;

int main() {
    ifstream in {"in.txt"};
    ofstream out {"out.txt"};

    if (!in.is_open()) {
        cerr << "file not found" << endl;
        return -1;
    }

    int lineNum = 1;
    string line;
    while (getline(in, line)) {
        if (lineNum % 2 == 0) {
            cout << line << endl;
        }
        lineNum ++;
    }
}