#include <cstdio>
#include <cctype>
#include <cstring>

enum Kind {
  K_INVALID = 0, K_DIRECTIVE, K_LABEL, K_ID, K_HEXINT, K_REG, K_DEC, K_COMMA, K_LPAREN, K_RPAREN
};

static inline bool is_alpha_(unsigned char c){ return std::isalpha(c)!=0; }
static inline bool is_digit_(unsigned char c){ return std::isdigit(c)!=0; }
static inline bool is_alnum_(unsigned char c){ return std::isalnum(c)!=0; }
static inline bool is_space_(unsigned char c){ return c==' ' || c=='\t' || c=='\n' || c=='\r'; }
static inline bool is_hex_(unsigned char c){
  return is_digit_(c) || (c>='a'&&c<='f') || (c>='A'&&c<='F');
}

static const char* kind_str(Kind k){
  switch(k){
    case K_DIRECTIVE: return "DIRECTIVE";
    case K_LABEL:     return "LABEL";
    case K_ID:        return "ID";
    case K_HEXINT:    return "HEXINT";
    case K_REG:       return "REG";
    case K_DEC:       return "DEC";
    case K_COMMA:     return "COMMA";
    case K_LPAREN:    return "LPAREN";
    case K_RPAREN:    return "RPAREN";
    default:          return "INVALID";
  }
}

static Kind classify(const char* buf, int len){
  if (len<=0) return K_INVALID;
  if (len==1 && buf[0]==',') return K_COMMA;
  if (len==1 && buf[0]=='(') return K_LPAREN;
  if (len==1 && buf[0]==')') return K_RPAREN;

  if (buf[0]=='.' && len>=2 && is_alpha_((unsigned char)buf[1])){
    for (int i=2;i<len;i++) if(!is_alnum_((unsigned char)buf[i])) return K_INVALID;
    return K_DIRECTIVE;
  }
  if (buf[len-1]==':' && is_alpha_((unsigned char)buf[0])){
    for (int i=1;i<len-1;i++) if(!is_alnum_((unsigned char)buf[i])) return K_INVALID;
    return K_LABEL;
  }
  if (is_alpha_((unsigned char)buf[0])){
    for (int i=1;i<len;i++) if(!is_alnum_((unsigned char)buf[i])) return K_INVALID;
    return K_ID;
  }
  if (len>=3 && buf[0]=='0' && (buf[1]=='x' || buf[1]=='X')){
    for (int i=2;i<len;i++) if(!is_hex_((unsigned char)buf[i])) return K_INVALID;
    return K_HEXINT;
  }
  if (buf[0]=='$'){
    if (len==2 && is_digit_((unsigned char)buf[1])) return K_REG;
    if (len==3 && is_digit_((unsigned char)buf[1]) && is_digit_((unsigned char)buf[2])) return K_REG;
    return K_INVALID;
  }
  if (len==1 && buf[0]=='0') return K_DEC;
  if (buf[0]=='-'){
    if (len<=1 || buf[1]=='0') return K_INVALID;
    for (int i=1;i<len;i++) if(!is_digit_((unsigned char)buf[i])) return K_INVALID;
    return K_DEC;
  }
  if (is_digit_((unsigned char)buf[0]) && buf[0]!='0'){
    for (int i=1;i<len;i++) if(!is_digit_((unsigned char)buf[i])) return K_INVALID;
    return K_DEC;
  }
  return K_INVALID;
}

int main(){
  char line[8192];

  while (std::fgets(line, sizeof(line), stdin)){
    int L = (int)std::strlen(line);
    // strip trailing \r?\n if present
    if (L>0 && (line[L-1]=='\n' || line[L-1]=='\r')) line[--L] = '\0';
    if (L>0 && (line[L-1]=='\n' || line[L-1]=='\r')) line[--L] = '\0';

    int i = 0;
    while (i < L){
      while (i<L && is_space_((unsigned char)line[i])) ++i;
      if (i>=L) break;

      if (line[i]==',' || line[i]=='(' || line[i]==')'){
        char p = line[i++];
        char tok1[2] = {p, 0};
        Kind k1 = classify(tok1, 1);
        if (k1==K_INVALID){ std::fprintf(stderr,"ERROR\n"); return 0; }
        std::printf("%s %c\n", kind_str(k1), p);
        continue;
      }

      char tok[256]; int tlen = 0;
      while (i<L && !is_space_((unsigned char)line[i]) &&
             line[i]!=',' && line[i]!='(' && line[i]!=')')
      {
        if (tlen < (int)sizeof(tok)-1) tok[tlen++] = line[i];
        ++i;
      }
      tok[tlen] = '\0';

      Kind k = classify(tok, tlen);
      if (k==K_INVALID){ std::fprintf(stderr,"ERROR\n"); return 0; }
      std::printf("%s %s\n", kind_str(k), tok);
    }
  }
  return 0;
}
