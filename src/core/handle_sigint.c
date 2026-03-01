/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_sigint.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilypad <lilypad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 23:11:33 by ylang             #+#    #+#             */
/*   Updated: 2026/03/01 11:49:01 by lilypad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include "shell_context.h"

// record that sigint happened， this change is to be consumed by main process
// which is actaully quite current cmd and return to shell.
// change to new line because it's ugly to start new prompt on the same line
// write is signal safe
// write(STDOUT_FILENO, "\n", 1);
// write(STDOUT_FILENO, "h", 1);
// if (write(1, "\n", 1) == -1)
// 	return ;
// drop what users has entered into buffer of readline
// clear current readline buffer, not adding to history
// rl_replace_line("", 0);
// // fix readline inernal status, location of cursur, line start info,
// //	need to redisplay prompt or not
// rl_on_new_line();
// // redraw clean prompt
// rl_redisplay();
void	handle_sigint_in_prompt_mode(int sig_num)
{
	(void)sig_num;
	g_latest_signal_status = SIGINT;
	rl_done = 1;
}

/*
should stop collecting heredoc immediately, drop content of current heredoc,
entire cmd will not be executed anymore, shell return to main prompt,
	set exitcode to 130
*/
// 1/ record signal
// 2. output newline, to make it visually clean
// if (write(1, "\n", 1) == -1)
// 	return ;
// // 3. let readline exit/finish
// rl_replace_line("", 0);清空当前输入缓冲
// rl_on_new_line();
// rl_redisplay();
// rl_done = 1;  readline 立刻返回
void	handle_sigint_in_heredoc_mode(int sig_num)
{
	(void)sig_num;
	g_latest_signal_status = SIGINT;
	rl_replace_line("", 0);
	rl_done = 1;
	write(1, "\n", 1);
}
