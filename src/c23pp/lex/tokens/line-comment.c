#include <c23pp/lex/tokens/line-comment.h>
#include <c23pp/lex/tokens/newline.h>
#include <c23pp/lex/tokens/eof.h>

TOKEN_IMPL(TOKEN_LINE_COMMENT_START);
TOKEN_IMPL(TOKEN_LINE_COMMENT);

bool lex_is_line_comment(lex_impl_t * lex_impl) {
 return *lex_impl->cursor == '/'
  && *(lex_impl->cursor + 1) == '/';
}

char const * lex_handle_line_comment(lex_impl_t * lex_impl, bool * lexed) {
 //save shallow copy of lexer state so we can refer to it later
 lex_impl_t const start = *lex_impl;
 char const * err;

 //consume comment until newline or EOF
 size_t length = 0;
 lex_impl->cursor += 2;
 lex_impl->line_offset += 2;
 while (!lex_is_newline(lex_impl) && !lex_is_eof(lex_impl)) {
  length++;
  lex_impl->cursor++;
  lex_impl->line_offset++;
 }
 bool newline_token_lexed = false;
 err = lex_handle_newline(lex_impl, &newline_token_lexed);
 if (err) {
  return err;
 }
 if (!newline_token_lexed) {
  *lexed = false;
  return NULL;
 }

 //pop newline token so we can push it after the line comment token
 token_t token_newline;
 err = lex_pop_token(lex_impl, &token_newline);
 if (err) {
  return err;
 }

 //push line comment token and proceeding newline token
 token_t token_line_comment_start = {
  .type = TOKEN_LINE_COMMENT_START,
  .cursor = start.cursor,
  .line = start.line,
  .line_offset = start.line_offset,
  .length = 2
 };
 err = lex_push_token(lex_impl, &token_line_comment_start);
 if (err) {
  return err;
 }
 token_t token_line_comment = {
  .type = TOKEN_LINE_COMMENT,
  .cursor = start.cursor + 2,
  .line = start.line,
  .line_offset = start.line_offset + 2,
  .length = length
 };
 err = lex_push_token(lex_impl, &token_line_comment);
 if (err) {
  return err;
 }
 err = lex_push_token(lex_impl, &token_newline);
 if (err) {
  return err;
 }
 *lexed = true;

 /*NOTE: no need to advance lexer here since the loop above will advance the
  *lexer to one past the end of the newline token
  */

 return NULL;
}
