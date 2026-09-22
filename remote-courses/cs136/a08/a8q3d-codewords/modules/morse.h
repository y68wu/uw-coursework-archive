// This module provides the Morse Code alphabet in SOS format

#include "sos.h"

// build_morse_codebook() provides a the Morse codebook
// effects: allocates memory (caller must call sos_destroy)
struct sos *build_morse_codebook(void);
