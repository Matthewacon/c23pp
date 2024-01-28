#define C23PP_INTERNAL
#include <c23pp/lex/internal.h>

#include <stdlib.h>
#include <string.h>
#include <errno.h>

lex_impl_t * lex_get_impl(lex_t * lex) {
 return (lex_impl_t *)lex;
}

char const * lex_get_file_length(size_t * dst, FILE * file) {
 //seek to end of file
 errno = 0;
 if (fseek(file, 0, SEEK_END)) {
  return strerror(errno);
 }

 //get cursor position
 errno = 0;
 *dst = ftell(file);
 if (errno) {
  return strerror(errno);
 }

 //reset cursor to beginning of file
 errno = 0;
 rewind(file);
 if (errno) {
  return strerror(errno);
 }

 return NULL;
}

char const * lex_self_guard(lex_impl_t * lex_impl) {
 if (((void *)lex_impl) == lex_impl->self) {
  return NULL;
 }

 return "FATAL: lexer instance is a shallow copy, refusing to mutate. Use "
  "`lex_copy()` to create a deep copy of the lexer before trying to produce a "
  "deep copy that can be mutated!";
}

char const * lex_push_token(lex_impl_t * lex_impl, token_t * token) {
 //guard against mutating shallow lexer copies
 char const * err = lex_self_guard(lex_impl);
 if (err) {
  return err;
 }

 //grow token stack, if necessary
 if (lex_impl->token_count + 1 >= lex_impl->token_size) {
  size_t new_size = lex_impl->token_size * 2;
  if (new_size < 10) {
   //NOTE: completely arbitrary
   new_size = 10;
  }
  token_t * copy = malloc(sizeof(token_t) * new_size);
  memcpy(
   copy,
   lex_impl->tokens,
   sizeof(token_t) * lex_impl->token_count
  );

  void * to_free = lex_impl->tokens;
  lex_impl->tokens = copy;
  lex_impl->token_size = new_size;
  free(to_free);
 }

 lex_impl->tokens[lex_impl->token_count] = *token;
 lex_impl->token_count++;

 return NULL;
}

char const * lex_pop_token(lex_impl_t * lex_impl, token_t * dst) {
 //guard against mutating shallow lexer copies
 char const * err = lex_self_guard(lex_impl);
 if (err) {
  return err;
 }

 //error if there are no tokens
 if (!lex_impl->token_count) {
  return "No tokens left";
 }

 //pop token and decrement token count
 *dst = lex_impl->tokens[lex_impl->token_count];
 lex_impl->token_count--;

 return NULL;
}

bool lex_has_tokens(lex_impl_t * lex_impl) {
 return lex_impl->token_count;
}
