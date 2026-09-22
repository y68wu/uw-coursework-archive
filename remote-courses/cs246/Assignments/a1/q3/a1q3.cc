import <iostream>;
import <fstream>;
import <sstream>;
import <string>;

using namespace std;

// count the characters of a string line
int count_chars(const string arr[]) {
    int count = 0;

    if (arr[0] == " ") {
        return 0;
    } 

    for (int i = 0; i < 10 && arr[i] != " "; i++) {
        count = count + (arr[i]).length() + 1;
    }

    count -= 1;
    return count;
}

int main(int argc, char* argv[]) {

    if (argc == 2) {
        int len;
        string input = argv[1];
        istringstream iss{input};
        iss >> len;

        string max_arr[10];
        string curr_arr[10];

        for (int i = 0; i < 10; i++) {
            max_arr[i] = " ";
            curr_arr[i] = " ";
        }

        string line; 
        int pos = 0;
        while(getline(cin, line)) {
            string word;
            istringstream iss{line};
            
            for (int i = 0; i < 10; i++) {
                curr_arr[i] = " ";
            }

            int arr_len = 0;
            while (iss >> word) {
                curr_arr[arr_len] = word;
                arr_len += 1;
            }

            for (int i = 0; i < arr_len; i++) {
                if ((max_arr[pos]).length() <= (curr_arr[i]).length()) {
                    max_arr[pos] = curr_arr[i];
                }
                pos += 1;
                pos = pos % len;
            }
        }

        int result = count_chars(max_arr);
        cout << result << endl;

    } else {
        string line;

        string max_arr[10];
        int max_len = 0;
        string curr_arr[10];
        int curr_len = 0;

        for (int i = 0; i < 10; i++) {
            max_arr[i] = " ";
            curr_arr[i] = " ";
        }

        bool first = true;
        
        while (getline(cin, line)) {
            if (first == true) {
                first = false;
                string word;
                istringstream iss{line};

                while (iss >> word) {
                    max_arr[max_len] = word;
                    max_len++;
                }

            } else {
                string word;
                istringstream iss{line};
                curr_len = 0;

                for (int i = 0; i < 10; i++) {
                    curr_arr[i] = " ";
                }

                while (iss >> word) {
                    curr_arr[curr_len] = word;
                    curr_len++;
                }

                for (int i = 0; i < 10; i++) {
                    if ((max_arr[i]).length() <= (curr_arr[i]).length()) {
                        max_arr[i] = curr_arr[i];
                    }
                }
            }
        }

        int result = count_chars(max_arr);

        cout << result << endl;
    }
}
