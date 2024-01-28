#include <c23pp/lex/lex.h>
#include <c23pp/lex/tokens/eof.h>
#include <c23pp/lex/tokens/newline.h>
#include <c23pp/lex/tokens/whitespace.h>
#include <c23pp/lex/tokens/line-comment.h>
#include <c23pp/lex/tokens/block-comment.h>
#include <c23pp/lex/tokens/hash.h>
#include <c23pp/lex/tokens/if-group.h>

#include <c23pp/lex/tokens/invalid.h>

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

//`lex_open` implementation
char const * lex_open(lex_t * lex, char const * path) {
 *lex = NULL;

 //open provided file path
 errno = 0;
 FILE * file = fopen(path, "r+");
 if (!file) {
  return strerror(errno);
 }

 //get file length
 size_t length;
 char const * error = lex_get_file_length(&length, file);
 if (error) {
  (void)fclose(file);
  return error;
 }

 //allocate buffer for file contents
 char * buf = malloc(length + 1);
 buf[length] = '\0';
 if (!buf) {
  (void)fclose(file);
  return "Failed to allocate buffer for file contents!";
 }
 errno = 0;
 size_t const read = fread(
  (void *)buf,
  1,
  length,
  file
 );
 (void)fclose(file);
 if (read != length) {
  return "Failed to read contents of file!";
 }

 //create new lexer
 lex_impl_t * result = malloc(sizeof(lex_impl_t));
 result->root = buf;
 result->cursor = buf;
 result->buffer_length = length;
 result->line = 0;
 result->line_offset = 0;
 result->self = result;
 *lex = result;

 return NULL;
}

//`lex_close` implementation
char const * lex_close(lex_t * lex) {
 if (!*lex) {
  return NULL;
 }
 char const * error = NULL;

 //always clear the user lex_t pointer first
 lex_impl_t * copy = lex_get_impl(*lex);
 *lex = NULL;

 //free underlying buffer and lex_t itself
 free((void *)copy->root);
 free(*lex);
 return error;
}

//TODO: `lex_copy` implementation
char const * lex_copy(lex_t * src, lex_t * dst) {
 (void)src;
 (void)dst;
 return "Unimplemented";
}

//TODO: `lex_next_token` implementation
typedef struct {
 bool (* check)(lex_impl_t * lex_impl);
 char const * (* handle)(lex_impl_t * lex_impl, bool * lexed);
} lexer_entry_t;
static lexer_entry_t lex_order[] = {
 //handle EOF
 {
  .check = lex_is_eof,
  .handle = lex_handle_eof
 },
 //handle newlines
 {
  .check = lex_is_newline,
  .handle = lex_handle_newline
 },
 //handle whitespace
 {
  .check = lex_is_whitespace,
  .handle = lex_handle_whitespace
 },
 //handle line comment
 {
  .check = lex_is_line_comment,
  .handle = lex_handle_line_comment
 },
 //handle block comment
 {
  .check = lex_is_block_comment,
  .handle = lex_handle_block_comment
 },
 //handle hash
 {
  .check = lex_is_hash,
  .handle = lex_handle_hash
 },
 //TODO: handle if-group (if, ifdef, ifndef)
 {
  .check = lex_is_if_group,
  .handle = lex_handle_if_group
 },
 //TODO: handle elif-group (elif, elifdef, elifndef)
 //TODO: handle else-group (else)
 //TODO: handle endif-line (endif)
 //TODO: handle control line

 //handle invalid tokens
 {
  .check = lex_is_invalid,
  .handle = lex_handle_invalid,
 },
};
char const * lex_next_token(lex_t * lex, token_t * dst) {
 char const * err;
 lex_impl_t * lex_impl = lex_get_impl(*lex);

 //if no tokens to pop, lex next token
 if (!lex_has_tokens(lex_impl)) {
  bool lexed_token = false;
  for (size_t i = 0; !lexed_token && i < sizeof(lex_order)/sizeof(*lex_order); i++) {
   //check if the cursor matches the current token
   if (!lex_order[i].check(lex_impl)) {
    continue;
   }

   //lex token
   err = lex_order[i].handle(lex_impl, &lexed_token);
   if (err) {
    return err;
   }
  }
 }

 //pop next token and return
 token_t to_return;
 err = lex_pop_token(lex_impl, &to_return);
 if (err) {
  return err;
 }
 *dst = to_return;
 return NULL;
}
