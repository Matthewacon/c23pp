#include <c23pp/lex/tokens/hash.h>

TOKEN_IMPL(TOKEN_HASH);

bool lex_is_hash(lex_impl_t * lex_impl) {
 return *lex_impl->cursor == '#';
}

char const * lex_handle_hash(lex_impl_t * lex_impl, bool * lexed) {
 //set up token
 token_t token_hash = {
  .type = TOKEN_HASH,
  .cursor = lex_impl->cursor,
  .line = lex_impl->line,
  .line_offset = lex_impl->line_offset,
  .length = 1
 };
 char const * err = lex_push_token(lex_impl, &token_hash);
 if (err) {
  return err;
 }
 *lexed = true;

 //advance lexer
 lex_impl->cursor++;
 lex_impl->line_offset++;

 return NULL;
}
