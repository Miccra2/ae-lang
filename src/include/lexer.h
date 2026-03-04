#ifndef __LEXER_H__
#define __LEXER_H__

#include <stdbool.h>

#include "utils.h"
#include "token.h"
#include "compiler.h"

#define LEX_NEWLINE(str)        (*(str) == '\n')
#define LEX_WHITESPACE(c)       (c == ' ' || c == '\n' || c == '\r' || c == '\t')
#define LEX_DIGIT(c)            (c >= '0' && c <= '9')
#define LEX_UPPER(c)            (c >= 'A' && c <= 'Z')
#define LEX_LOWER(c)            (c >= 'a' && c <= 'z')
#define LEX_ALPHA(c)            (LEX_UPPER(c) || LEX_LOWER(c))
#define LEX_IDENTIFIER_BEGIN(c) (c == '_' || LEX_ALPHA(c))
#define LEX_IDENTIFIER_END(c)   (LEX_DIGIT(c) || LEX_IDENTIFIER_BEGIN(c))

#define LEX_PATH         (state->path)
#define LEX_TEXT         (state->text.begin)
#define LEX_EOF          (state->text.end)
#define LEX_TOKENS       (state->tokens)
#define LEX_TOKEN_BUFFER (state->tokens.items)
#define LEX_POSITION     (state->position)
#define LEX_BEGIN        (state->position.begin)
#define LEX_END          (state->position.end)
#define LEX_OFFSET       (state->position.offset)
#define LEX_LINE         (state->position.line)
#define LEX_COLUMN       (position_column(state->position))
#define LEX_LENGTH       (posiiton_length(state->position))
#define LEX_CUR LEX_END
#define LEX_PTR LEX_BEGIN

Token generate_token(CompilerState *state);
void tokenise(CompilerState *state);

#endif // __LEXER_H__
