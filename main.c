#include <stdio.h>
#include <ctype.h>
#include <string.h>

typedef enum {
  TOKEN_END,
  TOKEN_INT,
  TOKEN_IDENT,
  TOKEN_ILLEGAL
} Token_Kind;

typedef struct {
  Token_Kind kind;
  const char* literal;
  size_t literal_len;
} Token;

typedef struct {
  const char* source;
  char ch;
  size_t source_len;
  size_t pos;
  size_t read_pos;
} Lexer;

void lexer_read_char(Lexer *l) {
  if (l->read_pos >= l->source_len) {
    l->ch = 0;
  } else {
    l->ch = l->source[l->read_pos];
  }
  l->pos = l->read_pos;
  l->read_pos += 1;
}

char lexer_peek_char(Lexer *l) {
  if (l->read_pos >= l->source_len) {
    return 0;
  }

  return l->source[l->read_pos];
}

void lexer_skip_whitespace(Lexer *l) {
  if (isspace(l->ch)) {
    lexer_read_char(l);
  }
}

Token lexer_next(Lexer *l) {
  Token tok = {0};
  lexer_skip_whitespace(l);
  switch (l->ch) {
  }
}

Lexer lexer_new(const char* source, size_t source_len) {
  Lexer lexer = {0};

  lexer.source = source;
  lexer.source_len = source_len;
  lexer_read_char(&lexer);

  return lexer;
}

int main(void) {
  const char *source = "print 'Hello mom'";
  Lexer l = lexer_new(source, strlen(source));
  printf("Lexer source: %s and length: %zu\nCurrent char: %c", l.source, l.source_len, l.ch);
}
