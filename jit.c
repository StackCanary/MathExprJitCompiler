#include "lexer.h"
#define true  1
#define false 0

#include "list.h"
#include "parser.h"
#include "code.h"
#include "run.h"

void repl()
{
	char buf[512];
	while (fgets(buf, 512, stdin) != NULL)
	{	
		char *pos = buf;

		list_t *token_list = create_list();

		while (true)
		{
			token_t token = lex(&pos);

			if (token.token == Undefined)
			{
				fprintf(stderr, "Error: Undefined token found\n");
				goto clean_up;
			}

			token_t *token_copy = malloc(sizeof(*token_copy));
			*token_copy = token;
			add_to_list(token_list, token_copy);
		
			if (token.token == Epsilon)
			{
				break;
			}


		}

		print_list(token_list);	


		expr_node_t *expr_node = NULL;
		if (parse(token_list, &expr_node)) 
		{
			printf("Valid Expression!\n");
			gen_code(expr_node);
			int size = size_code(expr_node);
			runnable_t job = make_job(size); 
			asm_code(expr_node, job);


			int ret = run(job);

			free_expr(expr_node);


			printf("Result %d\n", ret);

		} else
		{
				
			printf("Invalid Expression! :(\n");
		}

		clean_up:
			free_list(token_list);


	}

}

int main(void)
{
	repl();

	return 0;
}
