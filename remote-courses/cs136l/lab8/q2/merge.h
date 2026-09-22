#ifndef MERGE_H
#define MERGE_H
#include "doc.h"

// merge(doc1, doc2) creates a new document which appends doc2 to the
//   end of doc1.
//   The newly created document will be marked as unsigned.
//   A document returned by merge must be deleted by the user.
// require: doc1 and doc2 are not null
struct document *merge(struct document *doc1, struct document *doc2);
#endif
