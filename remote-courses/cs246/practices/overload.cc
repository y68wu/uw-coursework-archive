#include <iostream>
using namespace std;

struct Student {
    int id;
    int age;
    char gender;
};

// Overload the >> operator for the Student structure
istream& operator>>(istream& input, Student& s) {
    input >> s.id >> s.age >> s.gender;
    return input;
}

struct Posn{
    int x, y;
};

Posn operator+(Posn& p1, Posn& p2) {
    return Posn{p1.x+p2.x, p1.y+p2.y};
}

int main() {
    Student s;
    cout << "Enter student ID, age, and gender: ";
    cin >> s; // Now this line will work
    cout << "Student details - ID: " << s.id << ", Age: " << s.age << ", Gender: " << s.gender << endl;
    Posn p1{1,2};
    Posn p2{3,4};
    Posn p3 = p1 + p2;
}
