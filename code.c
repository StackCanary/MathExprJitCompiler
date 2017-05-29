#include "code.h"
#include <stdio.h>


void gen_code(expr_node_t *node)
{
	printf("pushq %rbp\n");
	printf("movq  %rsp  %rbp\n");

	gen_expr(node);

	printf("movq (%rsp) %rax\n");
	printf("movq  %rbp  %rsp\n");
	printf("popq  %rbp\n");
	printf("ret\n");
}


void gen_expr(expr_node_t *node)
{
	if (node)	
	{
		switch (node->tag)
		{	
			case EXPR_A: 
				gen_fact(node->a.fact);			
				gen_expr(node->a.expr);
				if (node->a.oper)
					printf("%s\n", token_to_string(node->a.oper->oper));
				break;
			case EXPR_B:
				gen_fact(node->b.fact);			
				break;
		}
				
	}
}

void gen_fact(fact_node_t *node)
{
	if (node)
	{
		switch (node->tag)	
		{
			case FACT_A:
				gen_term(node->a.term);
				gen_fact(node->a.fact);			
				if (node->a.oper)
					printf("%s\n", token_to_string(node->a.oper->oper));

				break;
			case FACT_B:
				gen_term(node->b.term);
				break;

		}
	}
}

void gen_term(term_node_t *node)
{
	if (node)
	{
		switch (node->tag)	
		{
			case TERM_A:
				gen_expr(node->a.expr);
				break;
			case TERM_B:
				if (node->b.numb)
					printf("pushq $%d\n", node->b.numb->numb);
				break;

		}
	}
}

int size_code(expr_node_t *node)
{
	return size_expr(node) + 0;
}

int size_expr(expr_node_t *node)
{
	if (node)	
	{
		switch (node->tag)
		{	
			case EXPR_A: 
				size_fact(node->a.fact) + size_expr(node->a.expr) + 1;
			case EXPR_B:
				return size_fact(node->b.fact);			
		}
				
	}
	
	return 0;
}

int size_fact(fact_node_t *node)
{
	if (node)
	{
		switch (node->tag)	
		{
			case FACT_A:
				return size_term(node->a.term) + size_fact(node->a.fact) + 1;
			case FACT_B:
				return size_term(node->b.term);

		}
	}

	return 0;
}

int size_term(term_node_t *node)
{
	if (node)
	{
		switch (node->tag)	
		{
			case TERM_A:
				return size_expr(node->a.expr);
			case TERM_B:
				return 1;
		}
	}

	return 0;

}
