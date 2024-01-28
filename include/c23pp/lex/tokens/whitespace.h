#pragma once

#include <c23pp/lex/tokens.h>
#define C23PP_INTERNAL
#include <c23pp/lex/internal.h>

bool lex_is_whitespace(lex_impl_t * lex_impl);
char const * lex_handle_whitespace(lex_impl_t * lex_impl, bool * lexed);
