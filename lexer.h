#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <fenv.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

typedef enum {
	Epsilon,
	Undefined,
	LParen,
	RParen,
	Number,
	Add,
	Mul,
	Sub,
	Div,
	Mod
} token_enum_t;

typedef struct {
	token_enum_t token;	
	int value;
} token_t;

token_t lex(char **pos);
int lex_number(char **pos);
char* token_to_string(token_enum_t token);

#define create_token_t(token) ((token_t) {(token), (0)})
#define number_token_t(token, number) ((token_t) {(token), (number)})


#endif
