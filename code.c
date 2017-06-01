#include "code.h"
#include <stdio.h>

#define EPILOG_SIZE 1
#define PROLOG_SIZE 1

void gen_code(expr_node_t *node)
{
	printf("pushq %%rbp\n");
	printf("movq  %%rsp  %%rbp\n");

	gen_expr(node);

	printf("movq (%%rsp) %%rax\n");
	printf("movq  %%rbp  %%rsp\n");
	printf("popq  %%rbp\n");
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

void asm_code(expr_node_t *node, runnable_t job)
{

	unsigned char *code = job.executable;

	// Push %rbp
	(*code++) = 0x55;

	// Mov %rsp, %rbp
	(*code++) = 0x48;
	(*code++) = 0x89;
	(*code++) = 0xe5;

	//push rdi
	(*code++) = 0x57;

	asm_expr(node, &code);

	// movq (%rsp), %rax
	(*code++) = 0x48;
	(*code++) = 0x8b;
	(*code++) = 0x04;
	(*code++) = 0x24;

	// pop rdi
	(*code++) = 0x5f;

	// movq %rbp, %rsp
	(*code++) = 0x48;
	(*code++) = 0x89;
	(*code++) = 0xec;

	// popq %rbp
	(*code++) = 0x5d;

	// ret
	(*code++) = 0xc3;
}


void asm_expr(expr_node_t *node, unsigned char **code)
{
	if (node)	
	{
		switch (node->tag)
		{	
			case EXPR_A: 
				asm_fact(node->a.fact, code);			
				asm_expr(node->a.expr, code);
				if (node->a.oper)
				{
					// pop rax
					(**code) = 0x58; (*code)++;
					// pop rdi	
					(**code) = 0x5f; (*code)++;
					// add rax, rdi
					(**code) = 0x48; (*code)++;
					(**code) = 0x01; (*code)++;
					(**code) = 0xf8; (*code)++;
					// push rax
					(**code) = 0x50; (*code)++;
				}
				break;
			case EXPR_B:
				asm_fact(node->b.fact, code);			
				break;
		}
				
	}
}

void asm_fact(fact_node_t *node, unsigned char **code)
{
	if (node)
	{
		switch (node->tag)	
		{
			case FACT_A:
				asm_term(node->a.term, code);
				asm_fact(node->a.fact, code);			
				if (node->a.oper)
				{
					// pop rdi	
					(**code) = 0x5f; (*code)++;
					// pop rax
					(**code) = 0x58; (*code)++;

					if (node->a.oper->oper == Mul)	
					{
						// add rax, rdi
						(**code) = 0x48; (*code)++;
						(**code) = 0xf7; (*code)++;
						(**code) = 0xe7; (*code)++;
					}

					if (node->a.oper->oper == Div)	
					{
						// xor rdx, rdx
						(**code) = 0x48; (*code)++;
						(**code) = 0x31; (*code)++;
						(**code) = 0xd2; (*code)++;
							
						// add rax, rdi
						(**code) = 0x48; (*code)++;
						(**code) = 0xf7; (*code)++;
						(**code) = 0xf7; (*code)++;
					}

					// push rax
					(**code) = 0x50; (*code)++;
				}
				break;
			case FACT_B:
				asm_term(node->b.term, code);
				break;

		}
	}
}

void asm_term(term_node_t *node, unsigned char **code)
{
	if (node)
	{
		switch (node->tag)	
		{
			case TERM_A:
				asm_expr(node->a.expr, code);
				break;
			case TERM_B:
				if (node->b.numb)
				{
					
					number_t number;
					number.number = node->b.numb->numb;
					
					if (node->b.numb->numb <= 127 )
					{
						(**code) = 0x6a;     (*code)++;
						(**code) = number.a; (*code)++;
					} else 
					{
						(**code) = 0x68; (*code)++;
						(**code) = number.a; (*code)++;
						(**code) = number.b; (*code)++;
						(**code) = number.c; (*code)++;
						(**code) = number.d; (*code)++;
					}

					// else	
				}
				break;

		}
	}
}

int size_code(expr_node_t *node)
{
	return 256;
}

int size_expr(expr_node_t *node)
{
	if (node)	
	{
		switch (node->tag)
		{	
			case EXPR_A: 
				return size_fact(node->a.fact) + size_expr(node->a.expr) + 1;
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
