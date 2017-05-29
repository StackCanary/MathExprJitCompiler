#ifndef CODE_H
#define CODE_H

#include "lexer.h"
#include "parser.h"

void gen_code(expr_node_t *node);
void gen_expr(expr_node_t *node);
void gen_fact(fact_node_t *node);
void gen_term(term_node_t *node);

int size_code(expr_node_t *node);
int size_expr(expr_node_t *node);
int size_fact(fact_node_t *node);
int size_term(term_node_t *node);

#endif
