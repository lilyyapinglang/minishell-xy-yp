/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_one_arg.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <xuewang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:37:42 by fvastena          #+#    #+#             */
/*   Updated: 2025/12/30 16:16:39 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/safefunctions.h"
#include "../include/parse.h"
#include "../include/minishellparse.h"

void expand_one_arg(char *str, t_list **expanded_args, t_shell_context *sh)
{
	t_expander exp;

	init_expander(&exp, str, expanded_args, sh);
	while (str[exp.i])
	{
		if (exp.context == NO_QUOTE)
			no_quote_state(str, &exp, sh);
		else if (exp.context == IN_SINGLE_QUOTE)
			single_quote_state(str, &exp);
		else if (exp.context == IN_DOUBLE_QUOTE)
			double_quote_state(str, &exp, sh);
		if (str[exp.i])
			exp.i++;
	}
	if (exp.context != NO_QUOTE)
		error("expand", "unexpected unclosed quote", EXIT_FAILURE, sh);
	add_token_to_list(&exp, sh);
}

void no_quote_state(char *str, t_expander *exp, t_shell_context *sh)
{
	if (str[exp->i] == '~')
		expand_tilde(str, exp, sh);
	else if (str[exp->i] == '$')
		expand_var(str, exp, sh);
	else if (str[exp->i] == ' ')
		add_token_to_list(exp, sh);
	else if (str[exp->i] == '\"') // here we are using \ to interprete " as a ", not \ itself
		exp->context = IN_DOUBLE_QUOTE;
	else if (str[exp->i] == '\'')
		exp->context = IN_SINGLE_QUOTE;
	// else if (str[exp->i] == '\\') //this we are handling \\
	//	exp->buf[exp->buf_i++] = str[++exp->i];
	else
		exp->buf[exp->buf_i++] = str[exp->i];
	// else
	//{
	//	if (str[exp->i] == '*')
	//		saved_wildcards_position("*", exp, sh);
	//	exp->buf[exp->buf_i++] = str[exp->i];
	// }
}

void single_quote_state(char *str, t_expander *exp)
{
	if (str[exp->i] == '\'')
	{
		if (exp->buf_i == 0)
			exp->empty_quotes = true;
		exp->context = NO_QUOTE;
	}
	else
		exp->buf[exp->buf_i++] = str[exp->i];
}

void double_quote_state(char *str, t_expander *exp, t_shell_context *sh)
{
	if (str[exp->i] == '$')
		expand_var(str, exp, sh);
	else if (str[exp->i] == '\"')
	{
		if (exp->buf_i == 0)
			exp->empty_quotes = true;
		exp->context = NO_QUOTE;
	}
	// else if (str[exp->i] == '\\'
	//	&& (str[exp->i + 1] == '\"' || str[exp->i + 1] == '\\'))
	//{
	//	exp->i++;
	//	exp->buf[exp->buf_i++] = str[exp->i];
	// }
	else
		exp->buf[exp->buf_i++] = str[exp->i];
}