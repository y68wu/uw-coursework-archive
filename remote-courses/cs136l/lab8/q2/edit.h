#ifndef EDIT_H
#define EDIT_H
#include "doc.h"

// edit(doc,index,s) will insert the provided sequence of characters at the sepecified index
// require: doc is not null
// require: index must be less than or equal to document's text
void edit(struct document *doc, int index, const char *s);
#endif
