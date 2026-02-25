/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_subshell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylang <ylang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 17:54:22 by ylang             #+#    #+#             */
/*   Updated: 2026/02/20 18:01:28 by ylang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*
典型语义：
父进程（主 shell）：
pid = fork()
waitpid(pid, &st)
用 check_process_child_exit(st, ...) 得到 subshell 的 status 并返回给 AND
子进程（subshell child）：
执行括号内整棵树：execute(subshell_child_tree, O_EXIT, sh)
结束后必须 exit（由 O_EXIT 保证）
所以现在出现第一个 fork：
*/

// run a smaller ast tree in subshell.
// turn waitstatus to $?
// if child process exit normally
// child process ended/interrupeted by signal

int	wait_status_to_exit_code(int wait_status)
{
	int	signal_num;

	if (WIFEXITED(wait_status))
		return (WEXITSTATUS(wait_status));
	else if (WIFSIGNALED(wait_status))
	{
		signal_num = WTERMSIG(wait_status);
		return (128 + signal_num);
	}
	else
		return (1);
}

// as long as i am in a forked child process,
//	i need to consider signal.
// child process need to exit otherwise we continue to run till waitpid
// even if sigint and sigout is ignored during exection process,
// waitpid could be
// interrupted by other signal that leads to return
//	-1 and errno set to EINTR
// so if waitpid return -1, & errn=EINTR, continue waiting
// other error , return error
int	execute_subshell(t_ast *node, t_shell_context *sh_ctx)
{
	pid_t			pid;
	int				status;
	t_ast_subshell	*subshell_node;

	subshell_node = &node->u_data.subshell;
	pid = fork();
	if (pid < 0)
		return (1);
	if (pid == 0)
	{
		sh_ctx->in_main_process = false;
		set_signal_in_exe_child_process();
		status = execute(subshell_node->child, RUN_IN_CHILD, sh_ctx);
		exit(status);
	}
	while (waitpid(pid, &status, 0) == -1)
	{
		if (errno == EINTR)
			continue ;
		return (1);
	}
	report_child_termination_signal(status, NULL, sh_ctx);
	status = wait_status_to_shell_status(status);
	return (status);
}
