#include "../include/token.h"

const char  *get_token_type_name(t_token_type type)
{
    if (type == TOKEN_WORD)
        return ("WORD");
    else if (type == TOKEN_PIPE)
        return ("PIPE");
    else if (type == TOKEN_SEMICOLON)
        return ("SEMICOLON ;");
    else if (type == TOKEN_LESS)
        return ("REDIRECT_IN < "); 
    else if (type == TOKEN_GREAT)
        return ("REDIRECT_OUT >"); 
    else if (type == TOKEN_DLESS)
        return ("REDIRECT_HEREDOC <<"); 
    else if (type == TOKEN_DGREAT)
        return ("REDIRECT_APPEND >> ");
    else if (type == TOKEN_EOF)
        return ("END_OF_FILE");
    else
        return ("UNKNOWN");
}

void    display_tokens(t_tokenlist *tokens)
{
    t_tokenlist *token;

    token = tokens;
    if (!token)
    {
        printf("--- No tokens generated ---\n");
        return ;
    }
    while (token)
    {
        const char *val = token->value ? token->value : "(null)";

        printf("Token: \033[0;36m %-20s \033[0m Type: \033[0;35m %-18s \033[0m \n",
            val, get_token_type_name(token->type));
    
        token = token->next;
    }
}


void    run_test(char *input)
{
    t_tokenlist *tokens;   printf("[DEBUG] After lexer_tokenize\n");

    printf("Input: \"%s\"\n", input);

    tokens = lexer_tokenize(input);
    display_tokens(tokens);
    token_list_free(tokens);
}
/*
int main(void)
{
    run_test("ls -l | grep 'file.c' > output");
    run_test("echo \"Hello World!\" < input.txt");
    run_test("command << EOF >> log.txt");
    run_test("no\\tspace\"test\"\'combined\'word");
    run_test("single_quotes_unclosed_start'");
    run_test("command | 'unclosed\"pipe");
    return (0);
}
*/

int	main(int argc, char **argv)
{
	t_tokenlist	*tokens;
	char	*input_command;

	if (argc != 2)
	{
		printf("Error: Invalid arguments.\n");
		printf("Usage: %s \"<command_to_tokenize>\"\n", argv[0]);
		return (1);
	}

	input_command = argv[1];

	printf("Input: \"%s\"\n", input_command);

	tokens = lexer_tokenize(input_command);

	display_tokens(tokens);

	token_list_free(tokens);

	return (0);
}