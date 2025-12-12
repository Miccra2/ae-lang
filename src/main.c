#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#define MACRO_BEGIN do {
#define MACRO_END   } while (0)

#define DA_MIN_CAP        512
#define DA_EMPTY(da)      (da){.capacity=0, .length=0, .items=NULL}
#define DA_BUF(da)        (da).items
#define DA_LEN(da)        (da).length
#define DA_GET(da, index) (da).items[index]

#define DA_ALLOC(da, length)                                                \
MACRO_BEGIN                                                                 \
    if ((da).capacity == 0) {                                               \
        (da).capacity = DA_MIN_CAP;                                         \
    }                                                                       \
    while ((da).capacity < length) {                                        \
        (da).capacity *= 2;                                                 \
    }                                                                       \
    (da).items = realloc((da).items, (da).capacity * sizeof(*(da).items));  \
MACRO_END

#define DA_PUSH(da, item)                                                   \
MACRO_BEGIN                                                                 \
    DA_ALLOC(da, (da).length + 1);                                          \
    (da).items[(da).length++] = item;                                       \
MACRO_END

#define DA_APPEND(da, items, count)                                         \
MACRO_BEGIN                                                                 \
    for (size_t i = 0; i < count; i++) {                                    \
        DA_PUSH(da, items[i]);                                              \
    }                                                                       \
MACRO_END

#define STRING_EMPTY DA_EMPTY(string)
#define STRING_CSTR(str) (&DA_GET(str, 0))

#define PATH       (s->path)
#define PATH_BUF   (PATH.items)
#define TEXT       (s->text)
#define TEXT_BUF   (TEXT.items)
#define POS        (s->position)
#define PTR        (POS.begin)
#define CUR        (POS.end)
#define LINE       (POS.line)
#define COLUMN     (POS.begin - POS.offset + 1)
#define TOKENS     (s->tokens)
#define TOKENS_BUF (TOKENS.items)
#define AST        (s->ast)
#define EXPRS      (AST.expressions)
#define EXPRS_BUF  (EXPRS.items)
#define VALUES     (AST.value_exprs)
#define VALUES_BUF (VALUES.items)
#define INFIXS     (AST.infix_exprs)
#define INFIXS_BUF (INFIXS.items)

#define WHITESPACES " \n\r\t"
#define DIGITS      "0123456789"

typedef struct string {
    size_t capacity;
    size_t length;
    char   *items;
} string;

///////////
// Position

typedef struct Position {
    size_t begin;
    size_t end;
    size_t offset;
    size_t line;
} Position;

/////////
// Tokens

typedef enum TokenKind {
    TOKEN_UNDEFINED,
    TOKEN_INTEGER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_SEMI_COLON,
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

/////////////////////////////
// Abstract Syntax Tree (AST)

typedef enum ExprKind {
    EXPR_UNDEFINED,
    EXPR_INFIX,
} ExprKind;

typedef struct Expression {
    ExprKind kind;
    Position position;
    size_t   index;
} Expression;

typedef struct Expressions {
    size_t     capacity;
    size_t     length;
    Expression *items;
} Expressions;

typedef struct ValueExpr {
    size_t capacity;
    size_t length;
    char   *items;
} ValueExpr;

typedef struct ValueExprs {
    size_t    capacity;
    size_t    length;
    ValueExpr *items;
} ValueExprs;

typedef enum InfixKind {
    INFIX_UNDEFINED,
    INFIX_PLUS,
    INFIX_MINUS,
} InfixKind;

typedef struct InfixExpr {
    InfixKind kind;
    size_t    l_expr;
    size_t    r_expr;
} InfixExpr;

typedef struct InfixExprs {
    size_t    capacity;
    size_t    length;
    InfixExpr *items;
} InfixExprs;

typedef struct AST {
    Exprs    expressions;   // dynamic array of expressions (value and infix operations)
    Values   value_exprs;   // dynamic array of values
    Infixs   infix_exprs;   // dynamic array of infix operations
} AST;

/////////////////
// Compiler State

typedef struct State {
    string   path;          // path to source code (".aec" extensions)
    string   text;          // text of source code
    Position position;      // for lexer current position of token and parser last position
    Tokens   tokens;        // dynamic array of tokens
    size_t   token_index;   // index into tokens for parsing AST
    AST      ast;           // Abstract Syntax Tree
} State;

string     str_from_cstr(char *cstr);
State      new_state(string path);
void       del_state(State *s);
bool       lex_ends(State *s, size_t offset);
char       lex_peek(State *s, size_t offset);
bool       lex_is(State *s, size_t offset, char *str);
bool       lex_has(State *s, size_t offset, char *c);
Token      lex_get_token(State *s);
void       lex_tokenise(State *s);
bool       tokens_end(State *s, size_t offset);
bool       expect_kind(State *s, size_t offset, TokenKind kind);
bool       expect_kinds(State *s, size_t offset, TokenKind *kinds, size_t count);
Expression parse_expression(State *s);
ValueExpr  parse_value_expr(State *s);
InfixExpr  parse_infix_expr(State *s);
void       parse(State *s);

string str_from_cstr(char *cstr) {
    string str = STRING_EMPTY;
    size_t len = 0;
    while (cstr[len++]);
    DA_APPEND(str, cstr, len);
    DA_PUSH(str, 0);
    return str;
}

State new_state(string path) {
    string buffer = STRING_EMPTY;
    FILE *pf = fopen(STRING_CSTR(path), "r");
    if (!pf) {
        fprintf(stderr, "[ERROR:%s] State: Could not open file!\n", STRING_CSTR(path));
        _exit(1);
    }
    fseek(pf, 0, SEEK_END);
    buffer.length = ftell(pf);
    DA_ALLOC(buffer, buffer.length);
    if (!buffer.items) {
        fprintf(stderr, "[ERROR:%s] State: Could not allocate text buffer!\n", STRING_CSTR(path));
        _exit(1);
    }
    rewind(pf);
    fread(buffer.items, 1, buffer.length, pf);
    fclose(pf);
    return (State){
        .path        = path,
        .text        = buffer,
        .position    = (Position){.begin=0, .end=0, .offset=0, .line=1},
        .tokens      = DA_EMPTY(Tokens),
        .token_index = 0,
        .ast         = {
            .expressions = DA_EMPTY(Exprs),
            .value_exprs = DA_EMPTY(Values),
            .infix_exprs = DA_EMPTY(Infixs),
        },
    };
}

void del_state(State *s) {
    if (TEXT_BUF) {
        free(TEXT_BUF);
    }
    TEXT = STRING_EMPTY;

    if (TOKENS_BUF) {
        free(TOKENS_BUF);
    }
    TOKENS = DA_EMPTY(Tokens);

    if (EXPRS_BUF) {
        free(EXPRS_BUF);
    }
    EXPRS = DA_EMPTY(Expressions);

    if (VALUES_BUF) {
        for (size_t i = 0; i < DA_LEN(VALUES_BUF); i++) {
            ValueExpr value_expr = *DA_GET(VALUES, i);
            if (value_expr.items) {
                free(value_expr.items);
            }
        }
        free(VALUES_BUF);
    }
    VALUES = DA_EMPTY(Values);

    if (INFIXS_BUF) {
        free(INFIXS_BUF);
    }
    INFIXS = DA_EMPTY(Infixs);
}

bool lex_ends(State *s, size_t offset) {
    return CUR + offset >= DA_LEN(TEXT);
}

char lex_peek(State *s, size_t offset) {
    if (lex_ends(s, offset)) {
        fprintf(stderr, "[ERROR:%zu:%zu:%s] Lexer: lex_peek trying to access s->text outside of bounds!\n", LINE, COLUMN, PATH_BUF);
        _exit(1);
    }
    return DA_GET(TEXT, CUR + offset);
}

bool lex_is(State *s, size_t offset, char *str) {
    size_t i = 0;
    while (!lex_ends(s, offset + i) && str[i]) {
        if (DA_GET(TEXT, CUR + offset + i) != str[i]) {
            return false;
        }
        i++;
    }
    return !str[i];
}

bool lex_has(State *s, size_t offset, char *c) {
    size_t i = 0;
    while (!lex_ends(s, offset) && c[i]) {
        if (DA_GET(TEXT, CUR + offset) == c[i]) {
            return true;
        }
        i++;
    }
    return false;
}

Token lex_get_token(State *s) {
    while (!lex_ends(s, 0)) {
        if (lex_has(s, 0, WHITESPACES)) {
            if (lex_peek(s, 0) == '\n') {
                LINE++;
                POS.offset = CUR + 1;
            }
            CUR++;
        } else {
            break;
        }
    }
    PTR = CUR;
    TokenKind kind = TOKEN_UNDEFINED;
    if (lex_ends(s, 0)) {
        return (Token){.kind=kind, .position=POS};
    }
    if (lex_peek(s, 0) == '+') {
        kind = TOKEN_PLUS;
    } else if (lex_peek(s, 0) == '-') {
        kind = TOKEN_MINUS;
    } else if (lex_peek(s, 0) == ';') {
        kind = TOKEN_SEMI_COLON;
    } else if (lex_has(s, 0, DIGITS)) {
        while (lex_has(s, 1, DIGITS)) {
            CUR++;
        }
        kind = TOKEN_INTEGER;
    } else {
        fprintf(stderr, "[ERROR:%zu:%zu:%s] Lexer: Encountered an unsupported character ", LINE, COLUMN, PATH_BUF);
        char c = lex_peek(s, 0);
        if (c >= 0x20 && c <= 0x7E) {
            fprintf(stderr, "'%c'!\n", c);
        } else {
            fprintf(stderr, "0x%02X!\n", c);
        }
    }
    CUR++;
    return (Token){.kind=kind, .position=POS};
}

void lex_tokenise(State *s) {
    Token token = lex_get_token(s);
    while (!lex_ends(s, 0) && token.kind != TOKEN_UNDEFINED) {
        DA_PUSH(TOKENS, token);
        token = lex_get_token(s);
    }
}

bool tokens_end(State *s, size_t offset) {
    return DA_LEN(TOKENS) > CUR + offset;
}

bool expect_kind(State *s, size_t offset, TokenKind kind) {
    return !tokens_end(s, offset) && DA_GET(TOKENS).kind == kind;
}

bool expect_kinds(State *s, size_t offset, TokenKind *kinds, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (expect_kind(s, offset + i, kinds[i])) {
            return false;
        }
    }
    return true;
}

Expression parse_expression(State *s) {}

ValueExpr parse_value_expr(State *s) {}

InfixExpr parse_infix_expr(State *s) {}

void parse(State *s) {}

int main() {
    string path = str_from_cstr("test.aec");
    State s = new_state(path);
    lex_tokenise(&s);
    parse(&s);
    printf("tokens: %zu\n", DA_LEN(s.tokens));
    printf("expressions: %zu\n", DA_LEN(s.ast.expressions));
    del_state(&s);
    return 0;
}
