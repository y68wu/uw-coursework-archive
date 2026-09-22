#include "sstream"
#include "iostream"

using namespace std;

void calculate(string line) {
    istringstream iss{line};
    int result;
    int num;
    char sym;
    iss >> result;

    while (iss >> sym >> num) {
        if (sym == '+') {
            result += num;
        } else {
            result -= num;
        }
    }
}

void output() {
    string line;
    while (getline(cin, line)) {
        calculate(line);
        cout << endl;
    }
}

int main() {

}
