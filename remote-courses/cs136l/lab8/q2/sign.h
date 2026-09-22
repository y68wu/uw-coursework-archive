#ifndef SIGN_H
#define SIGN_H
#include "doc.h"
#include <stdbool.h>

// sign(doc) will mark the document as signed
// require: doc is not null
void sign(struct document *doc);
#endif
