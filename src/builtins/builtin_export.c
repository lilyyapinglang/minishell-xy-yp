/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilypad <lilypad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 17:48:27 by lilypad           #+#    #+#             */
/*   Updated: 2026/02/16 18:29:17 by lilypad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void append_value()
{
    		{
			name = ft_substr(arg, 0, (size_t)(plus_eq - arg));
			val = equal_sign_loc + 1;
			if (!name || !is_valid_ident(name))
			{
				print_msg_n_return(1, "export", arg, "not a valid identifier");
				status = 1;
			}
			else if (env_append_value(ctx, name, val, true) != 0)
				status = 1;
			free(name);
		}
    
}

void set_new_value ()
{
    		{
			name = ft_substr(arg, 0, (size_t)(equal_sign_loc - arg));
			val = equal_sign_loc + 1;
			if (!name || !is_valid_ident(name))
			{
				print_msg_n_return(1, "export", arg, "not a valid identifier");
				status = 1;
			}
			else if (env_set_value(ctx, name, val, true) != 0)
				status = 1;
			free(name);
		}
}

int no_value()
{
     if (!is_valid_ident(arg))
			status=	print_msg_n_return(1, "export", arg, "not a valid identifier");
		else if (env_mark_exported(ctx, arg) != 0)
				status = 1;
}

char *get_plus_loc()
{
    if (equal_sign_loc && equal_sign_loc > arg && equal_sign_loc[-1] == '+')
			plus_eq = equal_sign_loc - 1;
}

// need to sort and print
// check each if --test=100
// check - valid options
/* Detect NAME+=VALUE safely:
		only when there is an '=' and the char right before '=' is '+' */
/* NAME+=VALUE */
/* value starts after '=' */
/* You may choose to print malloc-related error here */
/* NAME=VALUE */
/* NAME (export only) */

int	builtin_export(char **argv, t_shell_context *ctx)
{
	int		i;
	int		status;
	char	*equal_sign_loc;
	char	*plus_eq;

	i = 1;
	status = 0;
    plus_eq = NULL;
	if (!argv[1])
		return (print_env_or_export(true, ctx),0);
	while (argv[i])
	{
		if (argv[i][0] == '-')
			return (check_valid_options(argv[i], argv[0], "fnp"));
		equal_sign_loc = ft_strchr(arg, '=');
		plus_eq = *get_plus_loc();
		if (plus_eq)
           status= append_value();
		else if (equal_sign_loc)
           status= set_new_value ();
		else
            status = no_value();
		i++;
	}
	return (status);
}
