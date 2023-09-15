#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef enum {
  TOKEN_END,
  TOKEN_INT,
  TOKEN_IDENT,
  TOKEN_STRING,
  TOKEN_MINUS,
  TOKEN_PLUS,
  TOKEN_ILLEGAL
} Token_Kind;

typedef struct {
  Token_Kind kind;
  const char *literal;
  size_t literal_len;
} Token;

typedef struct {
  const char *source;
  char ch;
  size_t source_len;
  size_t pos;
  size_t read_pos;
} Lexer;

const char *token_kind_name(Token_Kind kind) {
  switch (kind) {
  case TOKEN_END:
    return "eof";
  case TOKEN_INT:
    return "integer";
  case TOKEN_IDENT:
    return "identifier";
  case TOKEN_STRING:
    return "string";
  case TOKEN_MINUS:
    return "minus";
  case TOKEN_PLUS:
    return "plus";
  case TOKEN_ILLEGAL:
    return "illegal";
  default:
    return NULL;
  }
}

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

char *lexer_read_string(Lexer *l, Token *t) {
  while (true) {
    lexer_read_char(l);
    t->literal_len += 1;
    if (l->ch == '"' || l->ch == '\'' || l->ch == 0) {
      break;
    }
  }
}

void lexer_read_identifier(Lexer *l, Token *t) {
  while (isalpha(l->ch)) {
    printf("read identifier: %c\n", l->ch);
    lexer_read_char(l);
    t->literal_len += 1;
  }
}

void lexer_read_number(Lexer *l, Token *t) {
  while (isdigit(l->ch)) {
    lexer_read_char(l);
    t->literal_len += 1;
  }
}

void lexer_skip_whitespace(Lexer *l) {
  while (isspace(l->ch)) {
    lexer_read_char(l);
  }
}

Token lexer_next(Lexer *l) {
  lexer_skip_whitespace(l);

  Token tok = {.literal = &l->source[l->pos]};
  switch (l->ch) {
  case '+': {
    tok.kind = TOKEN_PLUS;
    tok.literal_len = 1;
    lexer_read_char(l);
  } break;
  case '-': {
    tok.kind = TOKEN_MINUS;
    tok.literal_len = 1;
    lexer_read_char(l);
  } break;
  case '"': {
    tok.kind = TOKEN_STRING;
    lexer_read_string(l, &tok);
  } break;
  case 0: {
    tok.kind = TOKEN_END;
  } break;
  default:
    if (isalpha(l->ch)) {
      tok.kind = TOKEN_IDENT;
      lexer_read_identifier(l, &tok);
      return tok;
    } else if (isdigit(l->ch)) {
      tok.kind = TOKEN_INT;
      lexer_read_number(l, &tok);
      return tok;
    } else {
      tok.kind = TOKEN_ILLEGAL;
    }
  }

  lexer_read_char(l);
  return tok;
}

Lexer lexer_new(const char *source) {
  Lexer lexer = {0};

  lexer.source = source;
  lexer.source_len = strlen(source);
  lexer_read_char(&lexer);

  return lexer;
}

int main(void) {
  const char *source = "2 - 2";
  Lexer l = lexer_new(source);
  Token tok = lexer_next(&l);

  while (tok.kind != TOKEN_END) {
    printf("kind: %s, literal: %.*s\n", token_kind_name(tok.kind),
           (int)tok.literal_len, tok.literal);
    tok = lexer_next(&l);
  }

  return 0;
}
