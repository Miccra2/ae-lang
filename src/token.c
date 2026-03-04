#include "token.h"

Position new_position(char *begin, char *end, char *offset, size_t line) {
	return (Position){.begin=begin, .end=end, .offset=offset, .line=line};
}

Token new_token(TokenKind kind, Position position) {
	return (Token){.kind=kind, .position=position};
}

size_t position_length(Position position) {
	return position.end - position.begin;
}

size_t position_column(Position position) {
	return position.begin - position.offset + 1;
}

size_t token_length(Token token) {
	return position_length(token.position);
}

size_t token_column(Token token) {
	return position_column(token.position);
}
