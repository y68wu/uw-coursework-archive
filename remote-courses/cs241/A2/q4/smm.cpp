#include <iostream>
#include <sstream>
#include <string>
#include <vector>

const std::string ALPHABET    = ".ALPHABET";
const std::string STATES      = ".STATES";
const std::string TRANSITIONS = ".TRANSITIONS";
const std::string INPUT       = ".INPUT";
const std::string EMPTY       = ".EMPTY";

static inline bool isChar(const std::string &s)  { return s.size() == 1; }
static inline bool isRange(const std::string &s) { return s.size() == 3 && s[1] == '-'; }

struct Transition { int from; char c; int to; };

static int indexOf(const std::vector<std::string> &v, const std::string &x) {
  for (int i = 0; i < (int)v.size(); ++i) if (v[i] == x) return i;
  return -1;
}

int main() {
  std::istream &in = std::cin;
  std::string s;

  // ===== DFA data =====
  std::vector<std::string> stateNames;        // index -> name
  std::vector<int>         accepting;         // index -> 1/0
  int initialIdx = -1;
  std::vector<Transition>  edges;             // all transitions, order preserved

  //.ALPHABET
  std::getline(in, s); // skip header
  while (in >> s) {
    if (s == STATES) break;
    // expand single chars / ranges but we don't need to store them for SMM
  }

  // .STATES
  std::getline(in, s); // skip header
  bool seenInitial = false;
  while (in >> s) {
    if (s == TRANSITIONS) break;
    bool isAccept = false;
    if (s.size() > 1 && s.back() == '!') {
      isAccept = true;
      s.pop_back();
    }
    int idx = indexOf(stateNames, s);
    if (idx < 0) {
      stateNames.push_back(s);
      accepting.push_back(isAccept ? 1 : 0);
      idx = (int)stateNames.size() - 1;
    } else if (isAccept) {
      accepting[idx] = 1;
    }
    if (!seenInitial) { initialIdx = idx; seenInitial = true; }
  }

  //.TRANSITIONS
  std::getline(in, s); // skip header
  while (std::getline(in, s)) {
    if (s == INPUT) break;
    if (s.empty()) continue;

    std::istringstream line(s);
    std::vector<std::string> tok;
    while (line >> s) tok.push_back(s);
    if (tok.size() < 3) continue;

    const std::string &fromName = tok.front();
    const std::string &toName   = tok.back();

    int fromIdx = indexOf(stateNames, fromName);
    if (fromIdx < 0) {
      stateNames.push_back(fromName);
      accepting.push_back(0);
      fromIdx = (int)stateNames.size() - 1;
    }
    int toIdx = indexOf(stateNames, toName);
    if (toIdx < 0) {
      stateNames.push_back(toName);
      accepting.push_back(0);
      toIdx = (int)stateNames.size() - 1;
    }

    for (int i = 1; i < (int)tok.size() - 1; ++i) {
      const std::string &x = tok[i];
      if (isChar(x)) {
        edges.push_back({fromIdx, x[0], toIdx});
      } else if (isRange(x)) {
        for (char c = x[0]; c <= x[2]; ++c) edges.push_back({fromIdx, c, toIdx});
      }
    }
  }

  // Helper: last-declared-wins transition lookup
  auto nextState = [&](int st, char c) -> int {
    for (int i = (int)edges.size() - 1; i >= 0; --i) {
      if (edges[i].from == st && edges[i].c == c) return edges[i].to;
    }
    return -1;
  };

  // Read entire INPUT section as one long string
  std::string input;
  {
    std::string line;
    bool first = true;
    while (std::getline(in, line)) {
      if (!first) input.push_back('\n'); // preserve line breaks
      input += line;
      first = false;
    }
  }

  // Simplified Maximal Munch
  if (initialIdx < 0) { std::cerr << "ERROR\n"; return 0; }
  const int N = (int)input.size();
  int i = 0;

  while (i < N) {
    int cur = initialIdx;
    // last j where state was accepting
    int lastAcceptPos = -1;
    int j = i;

    while (j < N) {
      int nxt = nextState(cur, input[j]);
      // cannot consume input[j]
      if (nxt < 0) break;        
      cur = nxt;
      if (accepting[cur]) lastAcceptPos = j;
      ++j;
    }

    if (lastAcceptPos < i) {
      // never reached an accepting state from position i
      std::cerr << "ERROR\n";
      return 0;
    }

    // Output the lexeme [i, lastAcceptPos]
    std::cout << input.substr(i, lastAcceptPos - i + 1) << '\n';
    // restart from the next character
    i = lastAcceptPos + 1;       
  }

  return 0;
}
