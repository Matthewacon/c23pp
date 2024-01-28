#include <c23pp/lex/tokens/block-comment.h>
#include <c23pp/lex/tokens/newline.h>
#include <c23pp/lex/tokens/eof.h>

TOKEN_IMPL(TOKEN_BLOCK_COMMENT_OPEN);
TOKEN_IMPL(TOKEN_BLOCK_COMMENT_CLOSE);
TOKEN_IMPL(TOKEN_BLOCK_COMMENT);

bool lex_is_block_comment(lex_impl_t * lex_impl) {
 return *lex_impl->cursor == '/'
  && *(lex_impl->cursor + 1) == '*';
}

bool lex_is_block_comment_close(lex_impl_t * lex_impl) {
 return *lex_impl->cursor == '*'
  && *(lex_impl->cursor + 1) == '/';
}

char const * lex_handle_block_comment(lex_impl_t * lex_impl, bool * lexed) {
 char const * err;

 //push block comment open token
 token_t token_block_comment_open = {
  .type = TOKEN_BLOCK_COMMENT_OPEN,
  .cursor = lex_impl->cursor,
  .line = lex_impl->line,
  .line_offset = lex_impl->line_offset,
  .length = 2
 };
 err = lex_push_token(lex_impl, &token_block_comment_open);
 if (err) {
  return err;
 }

 /*advance lexer to beginning of block comment body and save shallow copy of
  *lexer state so we can refer to it later
  */
 lex_impl->cursor += 2;
 lex_impl->line_offset += 2;
 lex_impl_t const start = *lex_impl;

 //consume block comment until closing block comment or EOF
 size_t length = 0;
 while (!lex_is_block_comment_close(lex_impl) && !lex_is_eof(lex_impl)) {
  length++;
  lex_impl->cursor++;
  lex_impl->line_offset++;

  /*handle newlines so the length, line and line_offset values are correct, but
   *discard their tokens
   */
  if (!lex_is_newline(lex_impl)) {
   continue;
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

  //discard newline token
  token_t token_newline;
  err = lex_pop_token(lex_impl, &token_newline);
  if (err) {
   return err;
  }
  length += token_newline.length;
 }

 //push block comment body token
 token_t token_block_comment = {
  .type = TOKEN_BLOCK_COMMENT,
  .cursor = start.cursor,
  .line = start.line,
  .line_offset = start.line_offset,
  .length = length
 };
 err = lex_push_token(lex_impl, &token_block_comment);
 if (err) {
  return err;
 }

 //if we reached EOF before closing block comment token, lex next token
 if (lex_is_eof(lex_impl)) {
  *lexed = false;
  return NULL;
 }

 //push closing block comment token
 token_t token_block_comment_close = {
  .type = TOKEN_BLOCK_COMMENT_CLOSE,
  .cursor = lex_impl->cursor,
  .line = lex_impl->line,
  .line_offset = lex_impl->line_offset,
  .length = 2
 };
 err = lex_push_token(lex_impl, &token_block_comment_close);
 if (err) {
  return err;
 }
 *lexed = true;

 //advance lexer past block comment close
 lex_impl->cursor += 2;
 lex_impl->line_offset += 2;

 return NULL;
}
