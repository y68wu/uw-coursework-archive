import <iostream>;
import <fstream>;
import <string>;

using namespace std;

// differByOneChar checks if two words differ by exactly one character
bool differByOneChar(const string& word1, const string& word2) {
    if (word1.length() != word2.length()) {
        return false;
    }

    int length = word1.length();
    int diffCount = 0;
    for (int i = 0; i < length; i++) {
        if (word1[i] != word2[i]) {
            diffCount += 1;
            if (diffCount > 1) {
                return false;
            }
        }
    }

    return diffCount == 1;
}

// checks if a word can be played earlier
bool playEarlier(string arr[], string word, int len) {
    for (int i = 0; i < len; i++) {
        if (differByOneChar(arr[i], word)) {
            return true;
        }
    }

    return false;
}

// wordExists checks if a word exists in the word dictionary
bool wordExists(const string& word, const string& filename) {
    ifstream wordsfile{filename};
    string dicWord;

    while (wordsfile >> dicWord) {
        if (dicWord == word) {
            return true;
        }
    }
    
    return false;
}

int main(int argc, char* argv[]) {
    string startWord = argv[1];
    string endWord = argv[2];
    string wordsFile = argv[3];

    if (!wordExists(startWord, wordsFile) || !wordExists(endWord, wordsFile)) {
        cerr << "Error: Starting or ending word not found in words file" << endl;
        return 1;
    }
    
    int steps = 0;
    string arr[10];
    arr[0] = startWord;

    for (int i = 1; i < 10; i++) {
        arr[i] = " ";
    }
    
    string inputWord;
    string preWord = startWord;
    int bestScore = 0;
    bool begin = true;

    while (cin >> inputWord) {
        if (steps == 0 && begin) {
            cout << "Starting Word: " << startWord << endl;
        }
        
        begin = true;
        if (!differByOneChar(preWord, inputWord)) {
            cerr << "Error: " << inputWord << " does not differ from " << preWord << " by exactly one character" << endl;
            continue;
        }

        if (!wordExists(inputWord, wordsFile)) {
            cerr << "Error: " << inputWord << " does not belong to word file" << endl;

            continue;
        }

        if (playEarlier(arr, inputWord, steps)) {
            cerr << "This word could have been played earlier" << endl;
        } 

        int pos = steps + 1;
        arr[pos] = inputWord;
        preWord = inputWord;
        
        if (inputWord == endWord) {
            int score = 8 - steps;

            if (score > bestScore) {
                bestScore = score;
            }

            cout << "Congratulations! Your Score: " << score << ", Best Score: "<< bestScore << endl; 
            steps = 0;
            begin = false;
            cout << "Starting Word: " << startWord << endl;

            for (int i = 1; i < 10; i++) {
                arr[i] = " ";
            }
            
            preWord = startWord;
            continue;
        } else {
            steps += 1;
        }

        if (inputWord != endWord && steps == 8) {
            cout << "You lose" << endl;
            break;
        }
    }
}
