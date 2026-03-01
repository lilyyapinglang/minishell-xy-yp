/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilypad <lilypad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 22:06:28 by ylang             #+#    #+#             */
/*   Updated: 2026/03/01 11:49:01 by lilypad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*

builtin 是否在父进程/子进程跑，取决于调用 execute_command() 的那个进程是谁？”
**如果当前进程是“主 shell”，
那么：

external command → 必须 fork

stateful builtin → 不能 fork

stateless builtin → 可 fork 可不 fork**
*/
// Fallback: should not happen in normal minishell execution
int	wait_status_to_shell_status(int wait_status) {
  if (WIFEXITED(wait_status))
    return (WEXITSTATUS(wait_status));
  if (WIFSIGNALED(wait_status))
    return (128 + WTERMSIG(wait_status));
  if (WIFSTOPPED(wait_status))
    return (128 + WSTOPSIG(wait_status));
  return (1);
}

// only parent / interactive shell should print
void	report_child_termination_signal(int wait_status, const char *cmd_name,
                                     t_shell_context *ctx) {
  int sig;

  (void)cmd_name;
  if (ctx && ctx->in_main_process == false)
    return ;
  if (!WIFSIGNALED(wait_status))
    return ;
  sig = WTERMSIG(wait_status);
  if (sig == SIGINT)
    ft_putstr_fd("\n", STDERR_FILENO);
  else if (sig == SIGQUIT) {
    if (WCOREDUMP(wait_status))
      ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
    else
      ft_putstr_fd("Quit\n", STDERR_FILENO);
    ft_putstr_fd("Quit\n", STDERR_FILENO);
  }
}

// int		status;
/// reuse run in child logic
// status = execute(node, RUN_IN_CHILD, sh_ctx);
// printf("i'm in fork_and_run_in_child, before execute \n");
// printf("node now is: %s\n", node->u_data.command.args[1]);
// printf("i'm in fork_and_run_in_child, after execute \n");
// should be unreachable, safety net
int	fork_and_run_cmd_in_child(t_ast *node, t_shell_context *sh_ctx) {
  pid_t pid;
  int wait_status;

  pid = fork();
  if (pid < 0)
    return (print_errno_n_return(1, "fork", NULL, errno));
  if (pid == 0) {
    sh_ctx->in_main_process = false;
    set_signal_in_exe_child_process();
    execute(node, RUN_IN_CHILD, sh_ctx);
    exit(sh_ctx->last_status);
  }
  while (waitpid(pid, &wait_status, 0) == -1) {
    if (errno == EINTR)
      continue ;
    return (print_errno_n_return(1, "waitpid", NULL, errno));
  }
  report_child_termination_signal(wait_status, NULL, sh_ctx);
  return (wait_status_to_shell_status(wait_status));
}

// 这里理论上不应该发生（外面已经判断 is_builtin）
// 但为了鲁棒性，返回 1 或 0 都行；通常返回 1 更像“内部错误”
// 注意：builtin 函数自己负责打印错误并返回对应 statusms
// execute_builtin 不要 exit（除了 builtin_exit 自己 exit）
int	execute_builtin(t_ast_command *cmd, t_shell_context *ctx) {
  t_builtin_func func;

  if (!cmd || !cmd->args || !cmd->args[0])
    return (0);
  func = lookup_builtin_func(cmd->args[0]);
  if (!func)
    return (1);
  return (func(cmd->args, ctx));
}

// check if it is built-in
// if it is built-in like cd, export, unset, ecit et,
// run directly in currentg process
// if not buitle-in , like ls , grep,
// handover to fork_command decide how to run it
// builtin 总是在“当前进程”执行（谁调用它，它就在哪个进程里跑）
// already in child process, must not fork
// match the built_in name with built_in function name
// esle it is not built in, execve rewrite current process
// external: exec; if fails, return proper status (126/127/1)
// execve(get_cmd_path(cmd->args[0], sh_ctx), cmd->args,
// 	sh_ctx->env, sh_ctx);
// return (127);
// printf("I am in run_fork_wait\n ");
// has to be executed in parent process,shell process
// stateful builti has to be run in parent ,
//	otherwise change will not take effect
// need to fork , parent fork, child exe as run in child, parent wait
// default folk
// printf("I am in default fork \n ");
int	execute_command(t_ast *node, t_exec_context exe_ctx,
                    t_shell_context *sh_ctx) {
  bool isbuiltin;
  t_ast_command *cmd;

  cmd = &node->u_data.command;
  isbuiltin = false;
  if (!cmd->args || !cmd->args[0])
    return (EXIT_SUCCESS);
  isbuiltin = is_builtin(cmd->args[0]);
  if (exe_ctx == RUN_IN_CHILD) {
    if (isbuiltin)
      return (execute_builtin(cmd, sh_ctx));
    return (execute_external(cmd, sh_ctx));
  } else if (exe_ctx == RUN_FORK_WAIT)
    return (fork_and_run_cmd_in_child(node, sh_ctx));
  else if (exe_ctx == RUN_IN_SHELL) {
    if (isbuiltin && is_stateful_builtin(cmd->args[0]))
      return (execute_builtin(cmd, sh_ctx));
  }
  return (fork_and_run_cmd_in_child(node, sh_ctx));
}
