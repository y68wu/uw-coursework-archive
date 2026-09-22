import <fstream>;
import <cstring>;
using namespace std;

int main() {
  ofstream out{ "pgm.txt" }; // Open the output file "pgm.txt".
  // Mystery message.
  char text[] = {72, 111, 119, 32, 100, 111, 32, 73, 32, 99, 111, 109, 112, 105, 
      108, 101, 32, 117, 115, 105, 110, 103, 32, 109, 111, 100, 117, 108, 101, 
      115, 63, 0};

  out << "This will be fun!" << endl;
  for (size_t i = 0; i < strlen(text); ++i) {
    out << text[i];
  }
  out << endl;
  return 0;
}
