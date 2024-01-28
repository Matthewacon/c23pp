#pragma once

#include <c23pp/lex/tokens.h>
#define C23PP_INTERNAL
#include <c23pp/lex/internal.h>

bool lex_is_if_group(lex_impl_t * lex_impl);
char const * lex_handle_if_group(lex_impl_t * lex_impl, bool * lexed);
