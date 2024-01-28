#include <c23pp/lex/lex.h>

#include <stdio.h>

int main(void) {
 lex_t lexer = NULL;
 char const * error = lex_open(&lexer, "../example.c");
 if (error) {
  printf("Failed to create lexer: %s\n", error);
 }
 error = lex_close(&lexer);
 if (error) {
  printf("Failed to close lexer: %s\n", error);
 }
}
