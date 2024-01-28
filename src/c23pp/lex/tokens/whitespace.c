#include <c23pp/lex/tokens/whitespace.h>
#include <c23pp/lex/tokens/newline.h>
#include <c23pp/lex/tokens/eof.h>

#include <ctype.h>

TOKEN_IMPL(TOKEN_WHITESPACE);

bool lex_is_whitespace(lex_impl_t * lex_impl) {
 return isspace(*lex_impl->cursor)
  && !lex_is_newline(lex_impl)
  && !lex_is_eof(lex_impl);
}

char const * lex_handle_whitespace(lex_impl_t * lex_impl, bool * lexed) {
 //save shallow copy of lexer state so we can refer to it later
 lex_impl_t const start = *lex_impl;
 char const * err;

 //consume all whitespace
 size_t length = 0;
 while (lex_is_whitespace(lex_impl)) {
  //advance lexer
  length++;
  lex_impl->cursor++;
  lex_impl->line_offset++;

  //handle escaped newlines
  if (*lex_impl->cursor != '\\') {
   continue;
  }
  lex_impl->cursor++;
  lex_impl->line_offset++;

  //anything other than escaped newlines is not counted as whitespace
  if (!lex_is_newline(lex_impl)) {
   break;
  }
  bool newline_token_lexed;
  err = lex_handle_newline(lex_impl, &newline_token_lexed);
  if (err) {
   return err;
  }
  if (!newline_token_lexed) {
   *lexed = false;
   return NULL;
  }

  /*update whitespace length to include newline length
   *
   *NOTE: `lex_handle_newline` already advanced the lexer past the escaped
   *newline
   */
  token_t token_newline;
  err = lex_pop_token(lex_impl, &token_newline);
  if (err) {
   return err;
  }
  length += token_newline.length;
 }

 //set up token
 token_t token_whitespace = {
  .type = TOKEN_WHITESPACE,
  .cursor = start.cursor,
  .line = start.line,
  .line_offset = start.line_offset,
  .length = length
 };
 err = lex_push_token(lex_impl, &token_whitespace);
 if (err) {
  return err;
 }

 /*NOTE: no need to advance lexer here since the loop above will advance the
  *lexer to one past the end of the whitespace token
  */

 return NULL;
}
