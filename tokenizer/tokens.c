/**
 * Tokenizer for a simple shell language.
 *
 * Tokens:
 * ( ) < > ; |      special characters, each is a token on its own
 * "..."            a quoted string, the token is the text between the quotes
 * foo, bar_baz     words: sequences of non-special, non-whitespace characters
 *
 * Whitespace (space, tab, newline) separates tokens and is not part of any
 * token, unless it appears inside a quoted string.
 */
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "tokens.h"

/**
 * Is the given character whitespace?
 */
static int is_whitespace(char ch) {
  return ch == ' ' || ch == '\t' || ch == '\n';
}

/**
 * Is the given character a special (single-character token) character?
 */
static int is_special(char ch) {
  return ch != '\0' && strchr("()<>;|", ch) != NULL;
}

/**
 * Read a word from the input into the output buffer. A word ends at
 * whitespace, a special character, a quote, or the end of the input.
 * Returns the number of characters consumed from the input.
 */
static int read_word(const char *input, char *output) {
  int i = 0;
  while (input[i] != '\0' && !is_whitespace(input[i]) &&
         !is_special(input[i]) && input[i] != '"') {
    output[i] = input[i];
    ++i;
  }
  output[i] = '\0';

  return i;
}

/**
 * Read a quoted string from the input into the output buffer, without the
 * surrounding quotes. The input must start at the opening quote. If the
 * closing quote is missing, the string extends to the end of the line.
 * Returns the number of characters consumed from the input, including quotes.
 */
static int read_string(const char *input, char *output) {
  assert(input[0] == '"');

  int i = 1; // skip the opening quote
  while (input[i] != '\0' && input[i] != '\n' && input[i] != '"') {
    output[i - 1] = input[i];
    ++i;
  }
  output[i - 1] = '\0';

  if (input[i] == '"') {
    ++i; // skip the closing quote
  }

  return i;
}

/**
 * Split the given line into tokens and return them in a newly allocated
 * vector. The caller is responsible for deleting the vector.
 */
vect_t *tokenize(const char *input) {
  assert(input != NULL);

  vect_t *tokens = vect_new();
  // no token can be longer than the input itself
  char *buf = malloc(strlen(input) + 1);
  assert(buf != NULL);

  int i = 0; // current position in the input
  while (input[i] != '\0') {
    if (is_whitespace(input[i])) {
      ++i; // skip whitespace
    } else if (is_special(input[i])) {
      buf[0] = input[i];
      buf[1] = '\0';
      vect_add(tokens, buf);
      ++i;
    } else if (input[i] == '"') {
      i += read_string(&input[i], buf);
      vect_add(tokens, buf);
    } else {
      i += read_word(&input[i], buf);
      vect_add(tokens, buf);
    }
  }

  free(buf);
  return tokens;
}
