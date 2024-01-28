#include <c23pp/lex/tokens/newline.h>
#include <c23pp/lex/tokens/invalid.h>

TOKEN_IMPL(TOKEN_NEWLINE);

bool lex_is_newline(lex_impl_t * lex_impl) {
 char const * p = lex_impl->cursor;
 return *p == '\n'
  || (*p == '\r' && *(p+1) == '\n');
}

char const * lex_handle_newline(lex_impl_t * lex_impl, bool * lexed) {
 //save shallow copy of lexer state so we can refer to it later
 lex_impl_t const start = *lex_impl;

 size_t newline_length = 0;
 if (*lex_impl->cursor == '\n') {
  newline_length = 1;
 } else if (*lex_impl->cursor == '\r' && *(lex_impl->cursor+1) == '\n') {
  newline_length = 2;
 } else {
  //if malformed newline, return invalid token
  return lex_handle_invalid(lex_impl, lexed);
 }

 //set up token
 token_t token_newline = {
  .type = TOKEN_NEWLINE,
  .cursor = start.cursor,
  .line = start.line,
  .line_offset = start.line_offset,
  .length = newline_length
 };
 char const * err = lex_push_token(lex_impl, &token_newline);
 if (err) {
  return err;
 }
 *lexed = true;

 //advance lexer
 lex_impl->cursor += newline_length;
 lex_impl->line++;
 lex_impl->line_offset = 0;

 return NULL;
}
