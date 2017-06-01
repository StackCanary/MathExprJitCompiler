#ifndef CODE_H
#define CODE_H

#include "lexer.h"
#include "parser.h"
#include "run.h"

void gen_code(expr_node_t *node);
void gen_expr(expr_node_t *node);
void gen_fact(fact_node_t *node);
void gen_term(term_node_t *node);

void asm_code(expr_node_t *node, runnable_t job);
void asm_expr(expr_node_t *node, unsigned char **code);
void asm_fact(fact_node_t *node, unsigned char **code);
void asm_term(term_node_t *node, unsigned char **code);

int size_code(expr_node_t *node);
int size_expr(expr_node_t *node);
int size_fact(fact_node_t *node);
int size_term(term_node_t *node);

typedef union
{
	int number;

	struct
	{
		unsigned char a;	
		unsigned char b;	
		unsigned char c;	
		unsigned char d;	
	};

} number_t;


#endif
