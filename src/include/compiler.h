#ifndef __COMPILER_H__
#define __COMPILER_H__

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

#include "utils.h"
#include "token.h"

typedef struct CompilerState {
	char     *path;
	string   text;
	Position position;
	Tokens   tokens;
} CompilerState;

CompilerState compiler_state_from_path(char *path);
void delete_compiler(CompilerState *state);

#endif // __COMPILER_H__
