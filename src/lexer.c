#include "lexer.h"

Token generate_token(CompilerState *state) {
	while (LEX_CUR < LEX_EOF) {
		if (LEX_WHITESPACE(*LEX_CUR)) {
			if (LEX_NEWLINE(LEX_CUR)) {
				LEX_LINE++;
				LEX_OFFSET = LEX_CUR + 1;
			}
			LEX_CUR++;
		} else {
			break;
		}
	}
	LEX_PTR = LEX_CUR;
	if (LEX_CUR >= LEX_EOF) {
		return new_token(TOKEN_EOF, LEX_POSITION);
	}
	TokenKind kind = TOKEN_UNDEFINED;
	if (*LEX_CUR == '+') {
		kind = '+';
	} else if (*LEX_CUR == '-') {
		kind = '-';
	} else if (*LEX_CUR == ';') {
		kind = ';';
	} else if (LEX_DIGIT(*LEX_CUR)) {
		while (LEX_CUR + 1 < LEX_EOF && LEX_DIGIT(LEX_CUR[1])) {
			LEX_CUR++;
		}
		kind = TOKEN_INTEGER;
	} else if (LEX_IDENTIFIER_BEGIN(*LEX_CUR)) {
		while (LEX_CUR + 1 < LEX_EOF && LEX_IDENTIFIER_END(LEX_CUR[1])) {
			LEX_CUR++;
		}
		kind = TOKEN_IDENTIFIER;
	} else {
		fprintf(stderr, "Error: %zu:%zu:%s: Encountered an unsupported character ", LEX_LINE, LEX_COLUMN, LEX_PATH);
		char c = *LEX_CUR;
		if (c >= 0x20 && c <= 0x7E) {
			fprintf(stderr, "'%c'!\n", c);
		} else {
			fprintf(stderr, "0x%02hhX!\n", c);
		}
		exit(1);
	}
	LEX_CUR++;
	return new_token(kind, LEX_POSITION);
}

void tokenise(CompilerState *state) {
	Token token = generate_token(state);
	while (token.kind != TOKEN_EOF) {
		DA_PUSH(LEX_TOKENS, token);
		token = generate_token(state);
	}
}
