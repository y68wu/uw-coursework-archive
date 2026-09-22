#include <cstdio>
#include <string>
#include <cstdlib>

static const long long MAX_WLP4_INT = 2147483647LL;

static inline bool isLetter(int c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}
static inline bool isDigit(int c) {
  return (c >= '0' && c <= '9');
}

static inline void emit(const char* kind, const std::string& lex) {
  //kind + space + lex + '\n'
  fputs(kind, stdout);
  fputc(' ', stdout);
  fputs(lex.c_str(), stdout);
  fputc('\n', stdout);
}

static inline const char* keywordKind(const std::string& s) {
  //exact, case-sensitive
  if (s=="int")     return "INT";
  if (s=="wain")    return "WAIN";
  if (s=="if")      return "IF";
  if (s=="else")    return "ELSE";
  if (s=="while")   return "WHILE";
  if (s=="return")  return "RETURN";
  if (s=="println") return "PRINTLN";
  if (s=="putchar") return "PUTCHAR";
  if (s=="getchar") return "GETCHAR";
  if (s=="new")     return "NEW";
  if (s=="delete")  return "DELETE";
  if (s=="NULL")    return "NULL";
  return nullptr;
}

// Skip spaces/tabs/newlines/CR newline comments. We leave the first non-skipped character in the stream (via ungetc).
static void skipSpaceAndComments() {
  for (;;) {
    int c = getchar();
    // whitespace
    while (c == ' ' || c == '\t' || c == '\n' || c == '\r') c = getchar();
    if (c == '/') {
      int d = getchar();
      if (d == '/') {
        //consume until newline or EOF
        int e;
        while ((e = getchar()) != EOF && e != '\n') { /* skip */ }
        //continue outer loop to skip further whitespace and comments
        continue;
      } else {
        if (d != EOF) ungetc(d, stdin);
        ungetc('/', stdin);
        return;
      }
    } else {
      if (c != EOF) ungetc(c, stdin);
      return;
    }
  }
}

int main() {
  // Make stdio unbuffered to avoid libc allocating stream buffers
  setvbuf(stdin,  nullptr, _IONBF, 0);
  setvbuf(stdout, nullptr, _IONBF, 0);
  setvbuf(stderr, nullptr, _IONBF, 0);

  for (;;) {
    skipSpaceAndComments();
    int c = getchar();
    if (c == EOF) break;

    //identifier / Keyword
    if (isLetter(c)) {
      std::string lex;
      lex.push_back(static_cast<char>(c));
      for (;;) {
        int p = getchar();
        if (p == EOF) break;
        if (isLetter(p) || isDigit(p)) {
          lex.push_back(static_cast<char>(p));
        } else {
          ungetc(p, stdin);
          break;
        }
      }
      if (const char* k = keywordKind(lex)) emit(k, lex);
      else                                  emit("ID", lex);
      continue;
    }

    // Number
    if (isDigit(c)) {
      std::string lex;
      lex.push_back(static_cast<char>(c));
      for (;;) {
        int p = getchar();
        if (p == EOF) break;
        if (isDigit(p)) {
          lex.push_back(static_cast<char>(p));
        } else {
          ungetc(p, stdin);
          break;
        }
      }

      if (lex.size() > 1 && lex[0] == '0') { fputs("ERROR\n", stderr); return 0; }

      // Range check (≤ 2147483647)
      try {
        long long v = std::stoll(lex);
        if (v < 0 || v > MAX_WLP4_INT) { fputs("ERROR\n", stderr); return 0; }
      } catch (...) { fputs("ERROR\n", stderr); return 0; }
      emit("NUM", lex);
      continue;
    }

    //Two-char operators MM
    if (c == '=' || c == '!' || c == '<' || c == '>') {
      int d = getchar();
      if (d == '=') {
        if (c == '=') { emit("EQ", "==");  continue; }
        if (c == '!') { emit("NE", "!=");  continue; }
        if (c == '<') { emit("LE", "<=");  continue; }
        if (c == '>') { emit("GE", ">=");  continue; }
      }
      if (d != EOF) ungetc(d, stdin);
      if (c == '<') { emit("LT", "<");       continue; }
      if (c == '>') { emit("GT", ">");       continue; }
      if (c == '=') { emit("BECOMES", "=");  continue; }
      // lone '!' is illegal in WLP4
      fputs("ERROR\n", stderr); return 0;
    }

    // Single-char tokens
    switch (c) {
      case '(': emit("LPAREN","("); break;
      case ')': emit("RPAREN",")"); break;
      case '{': emit("LBRACE","{"); break;
      case '}': emit("RBRACE","}"); break;
      case '[': emit("LBRACK","["); break;
      case ']': emit("RBRACK","]"); break;
      case ',': emit("COMMA", ","); break;
      case ';': emit("SEMI",  ";"); break;
      case '+': emit("PLUS",  "+"); break;
      case '-': emit("MINUS", "-"); break;
      case '*': emit("STAR",  "*"); break;
      case '/': emit("SLASH", "/"); break;
      case '%': emit("PCT",   "%"); break;
      case '&': emit("AMP",   "&"); break;
      default:
        fputs("ERROR\n", stderr); return 0;
    }
  }

  return 0;
}
