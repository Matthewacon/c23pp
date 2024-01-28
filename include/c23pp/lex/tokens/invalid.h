#pragma once

#include <c23pp/lex/tokens.h>
#define C23PP_INTERNAL
#include <c23pp/lex/internal.h>

bool lex_is_invalid(lex_impl_t * unused);
char const * lex_handle_invalid(lex_impl_t * lex_impl, bool * lexed);
