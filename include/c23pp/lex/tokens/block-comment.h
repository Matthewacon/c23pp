#pragma once

#include <c23pp/lex/tokens.h>
#define C23PP_INTERNAL
#include <c23pp/lex/internal.h>

bool lex_is_block_comment(lex_impl_t * lex_impl);
bool lex_is_block_comment_close(lex_impl_t * lex_impl);
char const * lex_handle_block_comment(lex_impl_t * lex_impl, bool * lexed);
