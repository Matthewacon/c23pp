#pragma once

#include <stddef.h>

//backing type for token types
typedef struct {
 char const * name;
} token_type_impl_t;

//global token definition pointer
typedef token_type_impl_t * token_type_t;

#define TOKEN_DECL(token_name) \
token_type_t token_name

#define TOKEN_IMPL(token_name) \
static token_type_impl_t local_##token_name = {\
 .name = #token_name\
};\
token_type_t token_name = &local_##token_name

//opaque lexer instance
typedef void * lex_t;

//token descriptor
typedef struct {
 //token type
 token_type_t type;
 //position of token in buffer
 char const * cursor;
 //source line information
 size_t
  line,
  line_offset;
 //token information
 size_t length;
} token_t;

char const * lex_open(lex_t * lex, char const * path);
char const * lex_close(lex_t * lex);
char const * lex_copy(lex_t * src, lex_t * dst);
char const * lex_next_token(lex_t * lex, token_t * dst);
