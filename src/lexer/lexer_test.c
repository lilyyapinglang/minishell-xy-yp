/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_lexer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: you                                           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30                                #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h> /* memset */
#include <stdlib.h>

#include "safefunctions.h"
#include "parse.h"
#include "minishellparse.h"

/*
   适配你“当前版本”的 token_type：
   TOKEN_WORD, TOKEN_LPAREN, TOKEN_RPAREN,
   TOKEN_PIPE, TOKEN_AND, TOKEN_OR,
   REDIR_INPUT, REDIR_OUTPUT, REDIR_APPEND, REDIR_HEREDOC,
   TOKEN_EOF, TOKEN_INVALID
*/

const char	*get_token_type_name(t_token_type type)
{
	if (type == TOKEN_WORD)
		return ("WORD");
	else if (type == TOKEN_LPAREN)
		return ("LPAREN (");
	else if (type == TOKEN_RPAREN)
		return ("RPAREN )");
	else if (type == TOKEN_PIPE)
		return ("PIPE |");
	else if (type == TOKEN_AND)
		return ("AND &&");
	else if (type == TOKEN_OR)
		return ("OR ||");
	else if (type == REDIR_INPUT)
		return ("REDIR_IN <");
	else if (type == REDIR_OUTPUT)
		return ("REDIR_OUT >");
	else if (type == REDIR_APPEND)
		return ("REDIR_APPEND >>");
	else if (type == REDIR_HEREDOC)
		return ("REDIR_HEREDOC <<");
#ifdef TOKEN_EOF
	else if (type == TOKEN_EOF)
		return ("EOF");
#endif
	else if (type == TOKEN_INVALID)
		return ("INVALID");
	return ("UNKNOWN");
}

static void	print_one_token_pretty(int i, t_list *node)
{
	t_token		*tk;
	const char	*val;

	tk = (t_token *)node->content;
	val = (tk && tk->value) ? tk->value : "(null)";

	/* 一行彩色：value + type */
	printf("Token[%02d]: \033[0;36m%-20s\033[0m  Type: \033[0;35m%-18s\033[0m\n",
		i, val, (tk ? get_token_type_name(tk->type) : "(null-token)"));
}

/* 彩色逐条展示 */
void	display_tokens(t_list *tokens)
{
	t_list	*cur;
	int		i;

	cur = tokens;
	i = 0;
	if (!cur)
	{
		printf("--- No tokens generated ---\n");
		return ;
	}
	while (cur)
	{
		print_one_token_pretty(i, cur);
		cur = cur->next;
		i++;
	}
}

/* 列表式展示 */
void	print_token_list(t_list *tokens)
{
	t_list	*cur;
	t_token	*tk;
	int		index;

	cur = tokens;
	index = 0;
	if (!cur)
	{
		printf("[Token List is EMPTY]\n");
		return ;
	}
	printf("------ TOKEN LIST ------\n");
	while (cur)
	{
		tk = (t_token *)cur->content;
		printf("[%02d] Type: %-18s  Value: %s\n",
			index,
			(tk ? get_token_type_name(tk->type) : "(null-token)"),
			(tk && tk->value) ? tk->value : "(null)");
		cur = cur->next;
		index++;
	}
	printf("------ END OF TOKENS -----\n");
}

/*
 * ⚠️ 释放策略说明（非常重要）：
 * - 如果你的 lexer/token/list 都是 “safe_alloc + tracking scope(PROMPT)” 管的：
 *     你不应该在这里手动 free，否则可能 double free。
 *     那就把 free_tokens_manual() 里的内容删掉，
 *     改成调用你项目的 “清 PROMPT scope” 函数（比如 free_scope(PROMPT, sh)）。
 *
 * - 如果你的 create_token 里用的是 ft_substr / malloc 系列，且 list 节点也是普通 malloc：
 *     那手动 free 是 OK 的。
 */

/*
static void	free_tokens_manual(t_list **tokens)
{
	t_list	*cur;
	t_list	*next;
	t_token	*tk;

	if (!tokens || !*tokens)
		return ;
	cur = *tokens;
	while (cur)
	{
		next = cur->next;
		tk = (t_token *)cur->content;
		if (tk)
		{
			free(tk->value);
			free(tk);
		}
		free(cur);
		cur = next;
	}
	*tokens = NULL;
}
*/
/* 单条输入：调用 lexer -> 打印 -> 释放 */
void	run_test(const char *input)
{
	t_shell_context	sh;
	t_list			*tokens;
	int				ret;

	printf("\n==============================\n");
	printf("Input: \"%s\"\n", input);
	printf("==============================\n");

	memset(&sh, 0, sizeof(sh));
	tokens = NULL;

	ret = lexer((char *)input, &tokens, &sh);
	printf("[lexer return] %d\n", ret);

	print_token_list(tokens);
	display_tokens(tokens);

	/* 释放：如果你工程用 tracking，请换成 free_scope(PROMPT, &sh) 之类 */
	//free_tokens_manual(&tokens);
}

/* main 两种模式：
   1) argv 模式：./lexer_test "cmd..."
   2) 内置用例模式：不传参就跑一组测试
*/
/*
int	main(int argc, char **argv)
{
	if (argc == 2)
	{
		run_test(argv[1]);
		return (0);
	}


	run_test("ls -l | grep 'file.c' > output");
	run_test("echo \"Hello\" && echo World");
	run_test("echo a||echo b");
	run_test("(echo a && echo b) || echo c");
	run_test("cat << EOF | wc >> out");
	run_test("cmd&&(echo x||echo y)|grep z");
	run_test("single_quotes_unclosed_start'");
	run_test("echo a & echo b");  

	return (0);
}
*/