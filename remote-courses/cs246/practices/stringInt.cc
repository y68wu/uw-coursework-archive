#include "iostream"
#include "sstream"

using namespace std;

void convertStringtoInt(string s) {
    if (istringstream iss{s}) {
        int num;
        if (iss >> num) {
            cout << num << endl;
        } else {
            cout << "cannot convert to string" << endl;
        }
    }
}

void convertInttoString(int num) {
    ostringstream oss;
    oss << num; //先把num输出到ostringstream中
    string str = oss.str(); //将ostringstream的num转化成std::string
    cout << "string is" << str << endl;
}

int main() {
    convertInttoString(123);
}