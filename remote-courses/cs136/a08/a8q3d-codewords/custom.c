// THIS IS OPTIONAL! 
// You can to create your own "custom" codebook here if you'd like

#include "sos.h"
#include "custom.h"

// see custom.h
struct sos *build_custom_codebook(void) {
  struct sos *seq = sos_create();
  for (int i = 0; i <= 127; ++i) {
    sos_insert_end(seq, "");
  }
  
  // sos_replace_at(seq, 'c', "codeword");

  return seq;
}
