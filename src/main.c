#include <stdio.h>

#include "utils.h"
#include "token.h"
#include "lexer.h"

char *token_kind_label[] = {
	['+']              = "PLUS",
	['-']              = "MINUS",
	[';']              = "SEMI_COLON",
	[TOKEN_UNDEFINED]  = "UNDEFINED",
	[TOKEN_EOF]        = "EOF",
	[TOKEN_INTEGER]    = "INTEGER",
	[TOKEN_IDENTIFIER] = "IDENTIFIER",
};

void token_info(CompilerState *state) {
	size_t len = LEX_TOKENS.length;
	printf("tokens: %zu\n", len);
	for (size_t i = 0; i < len; i++) {
		Token token = LEX_TOKEN_BUFFER[i];
		printf("Info: %zu:%zu:%s: Token(kind: %s(%zu), Position(begin: %zu, end: %zu, offset: %zu, line: %zu))\n", 
			token.position.line, token_column(token), LEX_PATH, token_kind_label[token.kind], (size_t)token.kind, 
			token.position.begin - LEX_TEXT, token.position.end - LEX_TEXT, token.position.offset - LEX_TEXT, token.position.line);
	}
}

int main() {
	char *path = "test.aec";
	CompilerState state = compiler_state_from_path(path);
	tokenise(&state);
	token_info(&state);
	delete_compiler(&state);
	return 0;
}
