/*
 * expr ->  fact + expr | fact
 * term -> (expr)       | number
 * fact ->  term * fact | term
 *
 */

#include "parser.h"

#define compose(x) { list_t *save = *stream; if (x) { return true; } else { *stream = save; } }

void free_expr(expr_node_t *node)
{
	if (node)	
	{
		switch (node->tag)
		{	
			case EXPR_A: 
				free_fact(node->a.fact);			

				if (node->a.oper)
					free(node->a.oper);

				free_expr(node->a.expr);
				break;
			case EXPR_B:
				free_fact(node->b.fact);			
				break;
		}
				
	}
}

void free_fact(fact_node_t *node)
{
	if (node)
	{
		switch (node->tag)	
		{
			case FACT_A:
				free_term(node->a.term);

				if (node->a.oper)
					free(node->a.oper);

				free_fact(node->a.fact);			
				break;
			case FACT_B:
				free_term(node->b.term);
				break;

		}
	}
}

void free_term(term_node_t *node)
{
	if (node)
	{
		switch (node->tag)	
		{
			case TERM_A:
				free_expr(node->a.expr);
				break;
			case TERM_B:
				if (node->b.numb)
					free(node->b.numb);
				break;

		}
	}
}

void stream_step(list_t **stream)
{
	*stream = (*stream)->next;
}

bool expect(list_t **stream, token_enum_t token)
{
	if (((*stream)->data)->token  == token)
	{
	//	printf("Parsed %s!\n", token_to_string(*((*stream)->data)));
		stream_step(stream); return true; 
	}

	return false;	
}

bool numb(list_t **stream, numb_node_t *numb_node)
{
	token_t *token = ((*stream)->data);

	if (token->token == Number)
	{
		numb_node->numb = token->value;	
		stream_step(stream); 
		return true; 
	}

	return false;
}

bool  mul_div(list_t **stream, oper_node_t *oper_node)
{
	token_t *token = ((*stream)->data);

	if (token->token == Mul || token->token == Div)
	{
		oper_node->oper = token->token;	
		stream_step(stream);
		return true;
	}

	return false;
}

bool  add_sub(list_t **stream, oper_node_t *oper_node)
{
	token_t *token = ((*stream)->data);

	if (token->token == Add)
	{
		oper_node->oper = token->token;	
		stream_step(stream);
		return true;
	}

	return false;
}

bool epsilon(list_t **stream)
{
	return expect(stream, Epsilon);
}


bool expr(list_t **stream, expr_node_t *expr_node)
{

	expr_node->tag    = EXPR_A;
	expr_node->a.fact = make_node(fact_node_t);
	expr_node->a.oper = make_node(oper_node_t);
	expr_node->a.expr = make_node(expr_node_t);

	compose(fact(stream, expr_node->a.fact) && add_sub(stream, expr_node->a.oper) && expr(stream, expr_node->a.expr));

	free(expr_node->a.fact);
	free(expr_node->a.oper);
	free(expr_node->a.expr);

	expr_node->tag    = EXPR_B;
	expr_node->b.fact = make_node(fact_node_t);

	compose(fact(stream, expr_node->b.fact));

	free(expr_node->b.fact);

 	return false;	
}

bool term(list_t **stream, term_node_t *term_node)
{

	term_node->tag    = TERM_A;
	term_node->a.expr = make_node(expr_node_t);
	
	compose(expect(stream, LParen) && expr(stream, term_node->a.expr) && expect(stream, RParen));

	free(term_node->a.expr);

	term_node->tag    = TERM_B;
	term_node->b.numb = make_node(numb_node_t);

	compose(numb(stream, term_node->b.numb));

	free(term_node->b.numb);

	return false;
}

bool fact(list_t **stream, fact_node_t *fact_node)
{

	fact_node->tag    = FACT_A;
	fact_node->a.term = make_node(term_node_t); 	
	fact_node->a.oper = make_node(oper_node_t); 	
	fact_node->a.fact = make_node(fact_node_t); 	

	compose(term(stream, fact_node->a.term) && mul_div(stream, fact_node->a.oper) && fact(stream, fact_node->a.fact));

	free(fact_node->a.term);
	free(fact_node->a.oper);
	free(fact_node->a.fact); 

	fact_node->tag    = FACT_B;
	fact_node->b.term = make_node(term_node_t); 	

	compose(term(stream, fact_node->b.term));

	free(fact_node->b.term);

	return false;
}

bool parse(list_t *list, expr_node_t **expr_node)
{
	list_t **stream = &(list->next);

	*expr_node = make_node(expr_node_t);

	compose(expr(stream, *expr_node) && epsilon(stream));

	free(*expr_node);
	
	return false;
}

