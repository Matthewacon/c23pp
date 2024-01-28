#include <c23pp/lex/tokens/eof.h>

TOKEN_IMPL(TOKEN_EOF);

bool lex_is_eof(lex_impl_t * lex_impl) {
 return *lex_impl->cursor == '\0';
}

char const * lex_handle_eof(lex_impl_t * lex_impl, bool * lexed) {
 //set up token
 token_t eof_token = {
  .type = TOKEN_EOF,
  .cursor = lex_impl->cursor,
  .line = lex_impl->line,
  .line_offset = lex_impl->line_offset,
  .length = 1
 };
 char const * err = lex_push_token(lex_impl, &eof_token);
 if (err) {
  return err;
 }
 *lexed = true;

 //NOTE: don't advance the lexer here since it is the end of the stream

 return NULL;
}
