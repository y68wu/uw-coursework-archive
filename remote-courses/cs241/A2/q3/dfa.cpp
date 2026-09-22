#include <iostream>
#include <sstream>
#include <string>
#include <vector>

const std::string ALPHABET    = ".ALPHABET";
const std::string STATES      = ".STATES";
const std::string TRANSITIONS = ".TRANSITIONS";
const std::string INPUT       = ".INPUT";
const std::string EMPTY       = ".EMPTY";

bool isChar(std::string s) {
  return s.length() == 1;
}
bool isRange(std::string s) {
  return s.length() == 3 && s[1] == '-';
}

// helper that avoid extra headers
static int indexOf(const std::vector<std::string> &v, const std::string &x) {
  for (int i = 0; i < (int)v.size(); ++i) if (v[i] == x) return i;
  return -1;
}
static bool contains(const std::vector<char> &v, char c) {
  for (char x : v) if (x == c) return true;
  return false;
}

struct Transition { int from; char c; int to; };

int main() {
  std::istream& in = std::cin;
  std::string s;

  // DFA storage
  std::vector<char> alphabet;
  std::vector<std::string> stateNames;
  std::vector<int> accepting; // 1 = accepting, 0 = not
  int initialIdx = -1;
  std::vector<Transition> transitions;

  std::getline(in, s); // Alphabet section (skip header)
  // Read characters or ranges separated by whitespace
  while(in >> s) {
    if (s == STATES) { 
      break; 
    } else {
      if (isChar(s)) {
        //// Variable 's[0]' is an alphabet symbol
        if (!contains(alphabet, s[0])) alphabet.push_back(s[0]);
      } else if (isRange(s)) {
        for(char c = s[0]; c <= s[2]; ++c) {
          //// Variable 'c' is an alphabet symbol
          if (!contains(alphabet, c)) alphabet.push_back(c);
        }
      } 
    }
  }

  std::getline(in, s); // States section (skip header)
  // Read states separated by whitespace
  while(in >> s) {
    if (s == TRANSITIONS) { 
      break; 
    } else {
      static bool initial = true;
      bool acceptingFlag = false;
      if (s.back() == '!' && !isChar(s)) {
        acceptingFlag = true;
        s.pop_back();
      }
      //// Variable 's' contains the name of a state
      stateNames.push_back(s);
      accepting.push_back(acceptingFlag ? 1 : 0);

      if (initial) {
        //// The state is initial
        initialIdx = (int)stateNames.size() - 1;
        initial = false;
      }
      if (acceptingFlag) {
        //// The state is accepting
        // (already recorded in accepting[])
      }
    }
  }

  std::getline(in, s); // Transitions section (skip header)
  // Read transitions line-by-line
  while(std::getline(in, s)) {
    if (s == INPUT) { 
      // Note: Since we're reading line by line, once we encounter the
      // input header, we will already be on the line after the header
      break; 
    } else if (!s.empty()) {
      std::string fromState, toState;
      std::istringstream line(s);
      std::vector<std::string> lineVec;
      while(line >> s) {
        lineVec.push_back(s);
      }
      if (lineVec.empty()) continue;

      fromState = lineVec.front();
      toState   = lineVec.back();

      // Build the symbol list contained in the middle tokens
      std::string symbols;
      for(int i = 1; i < (int)lineVec.size()-1; ++i) {
        std::string t = lineVec[i];
        if (isChar(t)) {
          symbols += t;
        } else if (isRange(t)) {
          for(char c = t[0]; c <= t[2]; ++c) symbols += c;
        }
      }

      int fromIdx = indexOf(stateNames, fromState);
      int toIdx   = indexOf(stateNames, toState);
      for ( char c : symbols ) {
        //// There is a transition from 'fromState' to 'toState' on 'c'
        if (fromIdx >= 0 && toIdx >= 0) {
          transitions.push_back({fromIdx, c, toIdx});
        }
      }
    }
  }

  auto nextState = [&](int st, char c) -> int {
    for (const auto &t : transitions) {
      if (t.from == st && t.c == c) return t.to;
    }
    return -1; // no transition
  };

  // Input section (already skipped header line itself)
  while(in >> s) {
    //// Variable 's' contains an input string for the DFA
    bool accepted = false;

    if (initialIdx < 0) { // no states defined
      accepted = false;
    } else if (s == EMPTY) {
      // Empty string: accept iff initial state is accepting
      accepted = (accepting[initialIdx] == 1);
      std::cout << EMPTY << ' ' << (accepted ? "true" : "false") << '\n';
      continue;
    } else {
      int cur = initialIdx;
      bool ok = true;

      for (char ch : s) {
        // (Out-of-alphabet symbols => reject)
        if (!contains(alphabet, ch)) { ok = false; break; }
        int nxt = nextState(cur, ch);
        if (nxt < 0) { ok = false; break; }
        cur = nxt;
      }
      accepted = ok && (accepting[cur] == 1);
    }

    std::cout << s << ' ' << (accepted ? "true" : "false") << '\n';
  }
}
