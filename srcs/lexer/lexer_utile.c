/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utile.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <xuewang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 00:48:40 by xueyan_wang       #+#    #+#             */
/*   Updated: 2025/12/30 16:29:19 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "safefunctions.h"
//#include "parse.h"
#include "minishellparse.h"

int	ft_isspace(int c)
{
	if (c == ' ' || c == '\t' || c == '\v' || c == '\n' || c == '\r'
		|| c == '\f')
		return (1);
	return (0);
}

int	ft_istoken(int c)
{
	if (c == '(' || c == ')' || c == '<' || c == '>' || c == '&' || c == '|')
		return (1);
	return (0);
}
