/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <xuewang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 20:07:58 by xuewang           #+#    #+#             */
/*   Updated: 2026/02/26 20:07:59 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MS_LIST_H
# define MS_LIST_H

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
	void *content;       // user-owned data
	struct s_list *next; // next node
	struct s_list *prev; // previous node
}	t_list;
#endif