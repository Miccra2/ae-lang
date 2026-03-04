#ifndef __AST_H__
#define __AST_H__

#include <stdint.h>

#include "token.h"

typedef enum ExprKind {
	EXPR_UNDEFINED,
	EXPR_VALUE,
	EXPR_INFIX,
} ExprKind;

typedef struct Expression {
	ExprKind kind;
	Position position;
} Expression;

typedef struct ValueExpr {
	uint64_t value;
} ValueExpr;

typedef enum InfixKind {
	INFIX_UNDEFINED,
	INFIX_PLUS,
	INFIX_MINUS,
} InfixKind;

typedef struct InfixExpr {
	InfixKind  kind;
	Expression *lhs;
	Expression *rhs;
} InfixExpr;

#endif // __AST_H__
