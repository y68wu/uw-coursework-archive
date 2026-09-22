#ifndef DOC_H
#define DOC_H
#include <stdbool.h>

struct document{
  char *text;
  bool signature;
};

//create_document() creates a new document. Any document created through this function
//  must be deleted once done.
struct document *create_document();

//delete_document(doc) deletes a previously created document
// require: doc must be non-null
void delete_document(struct document *doc);

//add_text(doc,s) will append the provided sequence of characters to the end of the document
// require: doc is non null, 
// require: s is a null-terminated character array
void add_text(struct document *doc, const char *s);

// print_document(doc) prints whether the document is signed and then the content
// require: doc is not null
void print_document(const struct document *doc);
#endif
