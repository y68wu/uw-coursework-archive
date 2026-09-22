// The Morse Code alphabet

#include "morse.h"

// see morse.h
struct sos *build_morse_codebook(void) {
  struct sos *seq = sos_create();
  for (int i = 0; i <= 127; ++i) {
    sos_insert_end(seq, "");
  }
  sos_replace_at(seq, 'A', ".-");
  sos_replace_at(seq, 'B', "-...");
  sos_replace_at(seq, 'C', "-.-.");
  sos_replace_at(seq, 'D', "-..");
  sos_replace_at(seq, 'E', ".");
  sos_replace_at(seq, 'F', "..-.");
  sos_replace_at(seq, 'G', "--.");
  sos_replace_at(seq, 'H', "....");
  sos_replace_at(seq, 'I', "..");
  sos_replace_at(seq, 'J', ".---");
  sos_replace_at(seq, 'K', "-.-");
  sos_replace_at(seq, 'L', ".-..");
  sos_replace_at(seq, 'M', "--");
  sos_replace_at(seq, 'N', "-.");
  sos_replace_at(seq, 'O', "---");
  sos_replace_at(seq, 'P', ".--.");
  sos_replace_at(seq, 'Q', "--.-");
  sos_replace_at(seq, 'R', ".-.");
  sos_replace_at(seq, 'S', "...");
  sos_replace_at(seq, 'T', "-");
  sos_replace_at(seq, 'U', "..-");
  sos_replace_at(seq, 'V', "...-");
  sos_replace_at(seq, 'W', ".--");
  sos_replace_at(seq, 'X', "-..-");
  sos_replace_at(seq, 'Y', "-.--");
  sos_replace_at(seq, 'Z', "--..");
  sos_replace_at(seq, 'a', ".-");
  sos_replace_at(seq, 'b', "-...");
  sos_replace_at(seq, 'c', "-.-.");
  sos_replace_at(seq, 'd', "-..");
  sos_replace_at(seq, 'e', ".");
  sos_replace_at(seq, 'f', "..-.");
  sos_replace_at(seq, 'g', "--.");
  sos_replace_at(seq, 'h', "....");
  sos_replace_at(seq, 'i', "..");
  sos_replace_at(seq, 'j', ".---");
  sos_replace_at(seq, 'k', "-.-");
  sos_replace_at(seq, 'l', ".-..");
  sos_replace_at(seq, 'm', "--");
  sos_replace_at(seq, 'n', "-.");
  sos_replace_at(seq, 'o', "---");
  sos_replace_at(seq, 'p', ".--.");
  sos_replace_at(seq, 'q', "--.-");
  sos_replace_at(seq, 'r', ".-.");
  sos_replace_at(seq, 's', "...");
  sos_replace_at(seq, 't', "-");
  sos_replace_at(seq, 'u', "..-");
  sos_replace_at(seq, 'v', "...-");
  sos_replace_at(seq, 'w', ".--");
  sos_replace_at(seq, 'x', "-..-");
  sos_replace_at(seq, 'y', "-.--");
  sos_replace_at(seq, 'z', "--..");
  sos_replace_at(seq, ' ', "-.-.-.-");
  sos_replace_at(seq, ',', "--..--");
  sos_replace_at(seq, ':', "---...");
  sos_replace_at(seq, ';', "-.-.-.");
  sos_replace_at(seq, '.', ".-.-.-");
  sos_replace_at(seq, '"', ".-..-.");
  sos_replace_at(seq, '(', "-----.");
  sos_replace_at(seq, ')', ".-----");
  sos_replace_at(seq, '\'', "-.--.-");
  sos_replace_at(seq, '1', ".----");
  sos_replace_at(seq, '2', "..---");
  sos_replace_at(seq, '3', "...--");
  sos_replace_at(seq, '4', "....-");
  sos_replace_at(seq, '5', ".....");
  sos_replace_at(seq, '6', "-....");
  sos_replace_at(seq, '7', "--...");
  sos_replace_at(seq, '8', "---..");
  sos_replace_at(seq, '9', "----.");
  sos_replace_at(seq, '0', "-----");

  return seq;
}
