// This module provides the NATO phonetic alphabet in SOS format

#include "sos.h"

// build_nato_codebook() provides a the NATO codebook
// effects: allocates memory (caller must call sos_destroy)
struct sos *build_nato_codebook(void);
