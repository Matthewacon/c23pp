#include <c23pp/lex/tokens/invalid.h>

TOKEN_IMPL(TOKEN_INVALID);

bool lex_is_invalid(lex_impl_t * unused) {
 (void)unused;
 return true;
}

char const * lex_handle_invalid(lex_impl_t * lex_impl, bool * lexed) {
 //set up token
 token_t token_invalid = {
  .type = TOKEN_INVALID,
  .cursor = lex_impl->cursor,
  .line = lex_impl->line,
  .line_offset = lex_impl->line_offset,
  .length = 1
 };
 lex_push_token(lex_impl, &token_invalid);
 *lexed = true;

 //advance lexer
 lex_impl->cursor++;
 lex_impl->line_offset++;

 return NULL;
}
