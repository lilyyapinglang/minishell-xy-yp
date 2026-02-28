/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylang <ylang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 20:07:58 by xuewang           #+#    #+#             */
/*   Updated: 2026/02/28 21:48:35 by ylang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIST_H
# define LIST_H

/**
 * Doubly linked list node.
 *
 * Generic container used throughout the shell to store:
 * - environment variables
 * - pipeline command lists
 * - allocated resource tracking
 *
 * `content` ownership depends on the usage context.
 */

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
	struct s_list	*prev;
}					t_list;
#endif