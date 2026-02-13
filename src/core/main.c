/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilypad <lilypad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 20:04:04 by lilypad           #+#    #+#             */
/*   Updated: 2026/02/11 20:05:12 by lilypad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// current at main process, main prompt, set ctrl-c, ctrl-d , ctrl-\-
// set signal first
/*配置“主进程”的信号行为
prompt 阶段 Ctrl-C / Ctrl-\ 的行为?
*/
// interactive shell setup (signals, state)
// final cleanup and exit

int	main(int argc, char **argv, char **envp)
{
	t_shell_context	shell_context;
	int				exit_status;

	(void)argv;
	if (argc != 1)
		exit(EXIT_FAILURE);
	set_signal_in_exe_main_process();
	init_shell(&shell_context, envp);
	exit_status = shell_repl_loop(&shell_context);
	shell_exit(&shell_context, exit_status);
}
