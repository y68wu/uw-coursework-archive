// The NATO phonetic alphabet

#include "nato.h"

// see nato.h
struct sos *build_nato_codebook(void) {
  struct sos *seq = sos_create();
  for (int i = 0; i <= 127; ++i) {
    sos_insert_end(seq, "");
  }
  sos_replace_at(seq, '\0', "Stop.");
  sos_replace_at(seq, ' ', "Space");
  sos_replace_at(seq, '1', "One");
  sos_replace_at(seq, '2', "Two");
  sos_replace_at(seq, '3', "Three");
  sos_replace_at(seq, '4', "Four");
  sos_replace_at(seq, '5', "Five");
  sos_replace_at(seq, '6', "Six");
  sos_replace_at(seq, '7', "Seven");
  sos_replace_at(seq, '8', "Eight");
  sos_replace_at(seq, '9', "Niner");
  sos_replace_at(seq, '0', "Zero");
  sos_replace_at(seq, '.', "Point");  
  sos_replace_at(seq, 'A', "Alpha");
  sos_replace_at(seq, 'B', "Bravo");
  sos_replace_at(seq, 'C', "Charlie");
  sos_replace_at(seq, 'D', "Delta");
  sos_replace_at(seq, 'E', "Echo");
  sos_replace_at(seq, 'F', "Foxtrot");
  sos_replace_at(seq, 'G', "Golf");
  sos_replace_at(seq, 'H', "Hotel");
  sos_replace_at(seq, 'I', "India");
  sos_replace_at(seq, 'J', "Juliet");
  sos_replace_at(seq, 'K', "Kilo");
  sos_replace_at(seq, 'L', "Lima");
  sos_replace_at(seq, 'M', "Mike");
  sos_replace_at(seq, 'N', "November");
  sos_replace_at(seq, 'O', "Oscar");
  sos_replace_at(seq, 'P', "Papa");
  sos_replace_at(seq, 'Q', "Quebec");
  sos_replace_at(seq, 'R', "Romeo");
  sos_replace_at(seq, 'S', "Sierra");
  sos_replace_at(seq, 'T', "Tango");
  sos_replace_at(seq, 'U', "Uniform");
  sos_replace_at(seq, 'V', "Victor");
  sos_replace_at(seq, 'W', "Whiskey");
  sos_replace_at(seq, 'X', "X-ray");
  sos_replace_at(seq, 'Y', "Yankee");
  sos_replace_at(seq, 'Z', "Zulu");
  sos_replace_at(seq, 'a', "Alpha");
  sos_replace_at(seq, 'b', "Bravo");
  sos_replace_at(seq, 'c', "Charlie");
  sos_replace_at(seq, 'd', "Delta");
  sos_replace_at(seq, 'e', "Echo");
  sos_replace_at(seq, 'f', "Foxtrot");
  sos_replace_at(seq, 'g', "Golf");
  sos_replace_at(seq, 'h', "Hotel");
  sos_replace_at(seq, 'i', "India");
  sos_replace_at(seq, 'j', "Juliet");
  sos_replace_at(seq, 'k', "Kilo");
  sos_replace_at(seq, 'l', "Lima");
  sos_replace_at(seq, 'm', "Mike");
  sos_replace_at(seq, 'n', "November");
  sos_replace_at(seq, 'o', "Oscar");
  sos_replace_at(seq, 'p', "Papa");
  sos_replace_at(seq, 'q', "Quebec");
  sos_replace_at(seq, 'r', "Romeo");
  sos_replace_at(seq, 's', "Sierra");
  sos_replace_at(seq, 't', "Tango");
  sos_replace_at(seq, 'u', "Uniform");
  sos_replace_at(seq, 'v', "Victor");
  sos_replace_at(seq, 'w', "Whiskey");
  sos_replace_at(seq, 'x', "X-ray");
  sos_replace_at(seq, 'y', "Yankee");
  sos_replace_at(seq, 'z', "Zulu");

  return seq;
}
