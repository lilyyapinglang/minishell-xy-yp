/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_single_argv.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <xuewang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 19:09:47 by xuewang           #+#    #+#             */
/*   Updated: 2025/12/30 19:09:59 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "safefunctions.h"
#include "parse.h"
#include "minishellparse.h"

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
	else
		exp->buf[exp->buf_i++] = str[exp->i];
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
	else
		exp->buf[exp->buf_i++] = str[exp->i];
}