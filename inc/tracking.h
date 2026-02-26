/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tracking.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <xuewang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 20:12:45 by xuewang           #+#    #+#             */
/*   Updated: 2026/02/25 20:12:46 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MS_TRACKING_H
# define MS_TRACKING_H

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
	ALLOC_UNTRACKED, // not tracked; freed manually
	ALLOC_SHELL,     // lifetime: whole shell
	ALLOC_PROMPT     // lifetime: one REPL iteration / one command line
}								t_tracking_scope;

#endif