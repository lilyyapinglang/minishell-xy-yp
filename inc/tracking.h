/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tracking.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylang <ylang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 20:08:47 by xuewang           #+#    #+#             */
/*   Updated: 2026/02/28 21:47:32 by ylang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TRACKING_H
# define TRACKING_H

/**
 * Allocation tracking group.
 *
 * ALLOC_UNTRACKED:
 *   Allocation is NOT added to allocated_pointers[]. owned elsewhere
 *   Caller (or another owner container like env list) must free it manually.
 * 		memory is owned and freed by the data structure (env, etc.)
 *
 * ALLOC_SHELL:
 *   shell-lifetime allocations freed once in quit_shell()
 *
 * ALLOC_PROMPT:
 *   Freed at the end of each prompt/command loop (clear_prompt()).
 *
 * Never track memory that is freed manually.
 */

typedef enum e_tracking_scope
{
	ALLOC_UNTRACKED,
	ALLOC_SHELL,
	ALLOC_PROMPT
}	t_tracking_scope;

#endif