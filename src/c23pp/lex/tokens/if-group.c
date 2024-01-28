#include <c23pp/lex/tokens/if-group.h>

#include <string.h>

//TODO: handle TOKEN_IF_GROUP
bool lex_is_if_group(lex_impl_t * lex_impl) {
 static char const * keywords[] = {
  "if",
  "ifdef",
  "ifndef"
 };

 size_t const buf_len = strlen(lex_impl->cursor);
 for (size_t i = 0; i < sizeof(keywords)/sizeof(*keywords); i++) {
  size_t const kw_len = strlen(keywords[i]);
  size_t const min_len = kw_len < buf_len ? kw_len : buf_len;
  if (!strncmp(lex_impl->cursor, keywords[i], min_len)) {
   return true;
  }
 }

 return false;
}

char const * lex_handle_if_group(lex_impl_t * lex_impl, bool * lexed) {
 (void)lex_impl;
 (void)lexed;

 return NULL;
}
