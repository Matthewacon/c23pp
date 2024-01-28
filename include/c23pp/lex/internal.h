#pragma once

/*NOTE: this header is technically private but I'm too lazy to set up header
 *separation for a prototype project so here's a header guard instead
 */
#ifndef C23PP_INTERNAL
 #error "This is an internal header, do not include it"
#endif

#include <c23pp/lex/lex.h>

#include <stdio.h>
#include <stdbool.h>

//internal implementation of lexer
typedef struct {
 char const
  * root,
  * cursor;
 size_t
  buffer_length,
  line,
  line_offset;
 //append-only stack of lexed tokens
 size_t
  token_count,
  token_size;
 token_t * tokens;
 //guard against accidentally mutating shallow-copies
 void * self;
} lex_impl_t;

lex_impl_t * lex_get_impl(lex_t * lex);
char const * lex_get_file_length(size_t * dst, FILE * file);
char const * lex_self_guard(lex_impl_t * lex_impl);
char const * lex_push_token(lex_impl_t * lex_impl, token_t * token);
char const * lex_pop_token(lex_impl_t * lex_impl, token_t * dst);
bool lex_has_tokens(lex_impl_t * lex_impl);
