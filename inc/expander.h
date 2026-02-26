/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <xuewang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 20:10:19 by xuewang           #+#    #+#             */
/*   Updated: 2026/02/26 20:10:23 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_H
# define EXPANDER_H

# include <stdbool.h>
# include "env.h"
# include "parse.h"
# include "shell_context.h"

typedef enum e_exp_context
{
	NO_QUOTE,
	IN_SINGLE_QUOTE,
	IN_DOUBLE_QUOTE
}	t_exp_context;

typedef struct s_expander
{
	t_exp_context	context;
	int				i;
	char			*buf;
	int				buf_size;
	int				buf_i;
	t_list			**tokens;
	bool			empty_quotes;
	t_list			*wildcards_position;
}	t_expander;

/* expander */
t_ast	*expander(t_ast *node, t_shell_context *sh);

void	expand_one_arg(char *str, t_list **expanded_args,
			t_shell_context *sh);
void	init_expander(t_expander *exp, char *str,
			t_list **expanded_args, t_shell_context *sh);

void	no_quote_state(char *str, t_expander *exp,
			t_shell_context *sh);
void	single_quote_state(char *str, t_expander *exp);
void	double_quote_state(char *str, t_expander *exp,
			t_shell_context *sh);

char	*expand_env_var(char *str, t_expander *exp,
			t_shell_context *sh);
char	*get_valid_name(char *str, t_expander *exp,
			t_shell_context *sh);

void	expand_tilde(char *str, t_expander *exp,
			t_shell_context *sh);
void	expand_var(char *str, t_expander *exp,
			t_shell_context *sh);

void	*add_token_to_list(t_expander *exp, t_shell_context *sh);

char	**convert_list_to_array(t_list **lst, t_shell_context *sh);
void	add_variable_to_buffer(char *value, t_expander *exp,
			t_shell_context *sh);
char	*word_splitting(t_expander *exp, char *value,
			t_shell_context *sh);

#endif