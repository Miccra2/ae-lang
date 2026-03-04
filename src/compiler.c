#include "compiler.h"

CompilerState compiler_state_from_path(char *path) {
	FILE *pf = fopen(path, "r");
	fseek(pf, 0, SEEK_END);
	size_t size = ftell(pf);
	char *buffer = malloc(size);
	rewind(pf);
	fread(buffer, 1, size, pf);
	fclose(pf);
	return (CompilerState){
		.path = path,
		.text = new_string(buffer, buffer + size),
		.position = new_position(buffer, buffer, buffer, 1),
		.tokens = EMPTY_DA(Tokens),
	};
}

void delete_compiler(CompilerState *state) {
	if (state->text.begin) {
		free(state->text.begin);
		state->text.begin = NULL;
		state->text.end = NULL;
		state->position.begin = NULL;
		state->position.end = NULL;
		state->position.offset = NULL;
	}
	if (state->tokens.items) {
		free(state->tokens.items);
		state->tokens.items = NULL;
		state->tokens.capacity = 0;
		state->tokens.length = 0;
	}
}
