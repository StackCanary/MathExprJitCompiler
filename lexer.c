#include "lexer.h"

char* token_to_string(token_enum_t token) {
	switch (token)	
	{
		case Epsilon  : return "Epsilon";
		case Undefined: return "Undefined";
		case LParen   : return "LParen";
		case RParen   : return "RParen";
		case Number   : return "Number ( )";
		case Add      : return "Add";
		case Mul      : return "Mul";
		case Sub      : return "Sub";
		case Div      : return "Div";
		case Mod      : return "Mod";
	}

	return "Undocumented token";
}

int is_number(char chr) 
{
	switch (chr)
       	{
		case '0':
		case '1': case '2': case '3':
		case '4': case '5': case '6':
		case '7': case '8': case '9':

			return true;
	}

	return false;
}

int char_to_num(char chr)
{
	return chr - '0';
}

int lex_number(char **pos)
{
	char *sav = *pos;

	while(is_number(**pos))
		(*pos)++;


	// TODO: use definition in stddef.h (ptrdiff_t) 
	unsigned int len = (*pos) - sav;

	int result = 0;
	for (int i = 0; i < len; i++)
	{
		int power = pow(10, i);

		if ( fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0) 
		{
			fprintf(stderr, "Error: unrepresentable digit.\n");
			exit(1);
		}

		int digit = char_to_num(sav[len - i - 1]);

		if (digit == 0)
			continue;

		if (power > INT_MAX / digit)
		{
			fprintf(stderr, "Error: unrepresentable digit (overflow).\n");
			exit(1);
		}

		result += digit * power;
	}

	return result;
}

token_t lex(char **pos) 
{

	while(**pos)	
	{
		switch (**pos)
		{
			case ' ' :
			case '\t':
			case '\n':
			case '\r':	
				(*pos)++; continue;

			case '(': (*pos)++; return create_token_t(LParen);
			case ')': (*pos)++; return create_token_t(RParen);
			case '+': (*pos)++; return create_token_t(Add);
			case '-': (*pos)++; return create_token_t(Sub);
			case '*': (*pos)++; return create_token_t(Mul);
			case '/': (*pos)++; return create_token_t(Div);
		//	case '%': (*pos)++; return create_token_t(Mod);

		}

		if (is_number(**pos))
		{
			return number_token_t(Number, lex_number(pos));
		}

		(*pos++);
		return create_token_t(Undefined);
	}

	return create_token_t(Epsilon);
}

