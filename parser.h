#ifndef PARSER_H
#define PARSER_H

typedef struct _parse_tree_t parse_tree_t;

#include "lexer.h"
#include "list.h"

typedef struct _expr_node_t expr_node_t;
typedef struct _term_node_t term_node_t;
typedef struct _fact_node_t fact_node_t;
typedef struct _numb_node_t numb_node_t;
typedef struct _oper_node_t oper_node_t; 

void expr_code_gen();
// etc


struct _numb_node_t
{
	int numb;
};


struct _oper_node_t
{
	token_enum_t oper;	
};

typedef enum 
{
	EXPR_A,
	EXPR_B
} expr_tag_t;

typedef struct
{
	fact_node_t *fact;			
	oper_node_t *oper;
	expr_node_t *expr;

} expr_a;

typedef struct 
{
	fact_node_t *fact;			
} expr_b;

struct _expr_node_t
{
	
	expr_tag_t tag;
		
	union
	{
		expr_a a;
		expr_b b;
	};
};


typedef enum
{
	TERM_A,
	TERM_B
} term_tag_t;


typedef struct
{
	expr_node_t *expr;
} term_a;

typedef struct
{
	numb_node_t *numb;
} term_b;

struct _term_node_t
{

	term_tag_t tag;
	
	union 
	{
		term_a a;	
		term_b b;
	};
};

/*
 * expr ->  fact + expr | fact   [x]
 * term -> (expr)       | number [X]
 * fact ->  term * fact | term   [X]
 *
 */

typedef enum 
{
	FACT_A,
	FACT_B
} fact_tag_t;

typedef struct 
{
	term_node_t *term;
	oper_node_t *oper;
	fact_node_t *fact;			
} fact_a;

typedef struct 
{
	term_node_t *term;			
} fact_b;

struct _fact_node_t
{
	
	fact_tag_t tag;
		
	union
	{
		fact_a a;
		fact_b b;
	};
};


bool expr(list_t **stream, expr_node_t *expr_node);
bool term(list_t **stream, term_node_t *term_node);
bool fact(list_t **stream, fact_node_t *fact_node);
bool numb(list_t **stream, numb_node_t *numb_node);
bool mul_div(list_t **stream, oper_node_t *oper_node);
bool add_sub(list_t **stream, oper_node_t *oper_node);

#define make_node(type) calloc(1, sizeof(type))

void free_expr(expr_node_t *);
void free_term(term_node_t *);
void free_fact(fact_node_t *);

bool parse(list_t *list, expr_node_t **expr_node);

#endif
