/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline_1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilypad <lilypad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 21:29:40 by ylang             #+#    #+#             */
/*   Updated: 2026/03/01 11:49:01 by lilypad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// bool	new_line;
// new_line = false;
/*
wait(&status) 可能返回 -1 并设置 errno = EINTR。
在你项目里（父进程在执行阶段可能 ignore SIGINT/SIGQUIT，但仍可能被其它信号打断），这是实际会发生的。
*/
// only do ui output to the last cmd
// report_child_termination_signal(status, &new_line, sh_ctx);
// pipeline &? last cmd exit code
// ui output ()

int	wait_for_children(pid_t last_pid, int count_pipeline,
		t_shell_context *sh_ctx)
{
	pid_t	child_pid;
	int		status;
	int		last_cmd_status;

	last_cmd_status = 0;
	while (count_pipeline > 0)
	{
		child_pid = wait(&status);
		if (child_pid == -1)
		{
			if (errno == EINTR)
				continue ;
			break ;
		}
		count_pipeline--;
		if (child_pid == last_pid)
		{
			report_child_termination_signal(status, NULL, sh_ctx);
			last_cmd_status = wait_status_to_shell_status(status);
		}
	}
	return (last_cmd_status);
}

int	execute_pipeline(t_ast *pipeline_node, t_shell_context *sh_ctx)
{
	t_list	*pipeline_list;

	if (!pipeline_node)
		return (EXIT_FAILURE);
	pipeline_list = build_cmd_list(pipeline_node, sh_ctx);
	return (execute_pipeline_commands(pipeline_list, sh_ctx));
}
