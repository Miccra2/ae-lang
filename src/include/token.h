#ifndef __TOKEN_H__
#define __TOKEN_H__

#include <stddef.h>

typedef struct Position {
	char   *begin;
	char   *end;
	char   *offset;
	size_t line;
} Position;

typedef enum TokenKind {
	TOKEN_UNDEFINED = 256,
	TOKEN_EOF,
	TOKEN_INTEGER,
	TOKEN_IDENTIFIER,
} TokenKind;

typedef struct Token {
	TokenKind kind;
	Position  position;
} Token;

typedef struct Tokens {
	size_t capacity;
	size_t length;
	Token  *items;
} Tokens;

Position new_position(char *begin, char *end, char *offset, size_t line);
Token new_token(TokenKind kind, Position position);
size_t position_length(Position position);
size_t position_column(Position position);
size_t token_length(Token token);
size_t token_column(Token token);

#endif // __TOKEN_H__
