#include "string"
#include "iostream"

using namespace std;

int main() {
    int num;
    char c;
    string word;
    string line;
    cin >> num >> c >> word;
    getline(cin, line);
    cout << "num is" << num << endl;
    cout << "c is" << c << endl;
    cout << "word is" <<word << endl;
    cout << "line is" <<line << endl;
}