/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_signal_handlers_to_mode.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilypad <lilypad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 21:26:29 by ylang             #+#    #+#             */
/*   Updated: 2026/03/01 11:49:01 by lilypad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include "shell_context.h"

// ssize_t read(int fd, void *buf, size_t count);

/*
press ctrl - c will trigger special char,
ask kernel to send signal SIGINT to current process
.asyn singnal
press ctrl-\ will trigger special char,
	ask kernel to send signal SIGQUIT to current process.async signal
press ctrl-D will not tigger signal,
	it represents EOF- "I'm no longer input any thing"
it does not represent a char, it will not enter buf,
	it will not trigger signal ctrl-d -> read() return (0); stdin reach eof,
	user_input = nUll,quit_shell()
*/

/* Depends on which stage/time when user press the keys,
	there could be different behaviors
There could be 4 stages :
1. When user press those keys when entering the normal prompt
2. When user press those keys when entering a heredoc prompt
3. When user press those keys when cmd is already running , in this case,
	there is parent process and child process
	3.1 defien how child process react to these 3 key presses
	3.2 define how parent process react to these 3 key presses
 */

/* prompt mode :
ctrl-c : g_signal_value = SIGINT , readline is interrrupted,
	back to interactive shell, status code =130, user_input ="" or null,
		clear prompt
ctrl-d : nothing happens, so ignore

*/

// write and read to this variable is atomatic, it won't be interrupted
//
#include <signal.h>

volatile sig_atomic_t	g_latest_signal_status = 0;

// Provide a general handler for each mode to call and use
// clean garbage value
// let calling function to decide if SA_RESTART
int	set_signal_handler(int sig_num, void (*signal_handler)(int), int flags)
{
	struct sigaction	sig_act;

	memset(&sig_act, 0, sizeof(sig_act));
	sig_act.sa_handler = signal_handler;
	sig_act.sa_flags = flags;
	sigemptyset(&sig_act.sa_mask);
	if (sigaction(sig_num, &sig_act, NULL) == -1)
		return (-1);
	return (0);
}

// set handling behavior for ctrl-c, SIGINT
// i can not kill shell, i need to clear line and return to prompt
// set handling behavior for ctrl-d, SIGQUIT, which is ignore,
//	nothing happens
void	set_signal_in_main_prompt_mode(void)
{
	set_signal_handler(SIGINT, handle_sigint_in_prompt_mode, 0);
	set_signal_handler(SIGQUIT, SIG_IGN, 0);
}

// set handling behavior for ctrl-c, SIGINT
// set handling behavior for ctrl-d, SIGQUIT, which is ignore,
//	nothing happens
void	set_signal_in_heredoc_prompt_mode(void)
{
	set_signal_handler(SIGINT, handle_sigint_in_heredoc_mode, 0);
	set_signal_handler(SIGQUIT, SIG_IGN, 0);
}

/* eg : sleep 100
when pressing ctrl-c, kernel send SIGINT to all foreground processes
both parent process and child process will received it.
if no signal handler is specifically defined,
	both parent and child process will execute
SIG_DFL , default behhavior,
SIGINT-> SIG_DFL , process will be immediately interrupeted/ terminiated
SIGQUIT->SIG_DFL,  process will be terminated + core dump
therefore, shell received SIGINT and die direcly ,
	we can't  return to prompy anymore,
so in parent, we don;t want sigint to shut down shell,
	we just want sigint to influence
the command that is currently running,
so for shell/main process itself, correct behavior is i don't respond,
	i will wait for what kids process returns for me,
update status, return to prompt  , shell is the manager but not the executor
*/
// set handling behavior for ctrl-c, as parent process ,
// i do nothing o react to signal, i will waitfor child process
// set handling behavior for ctrl-d, SIGQUIT, which is ignore,
//	nothing happens
void	set_signal_in_exe_main_process(void)
{
	set_signal_handler(SIGINT, SIG_IGN, 0);
	set_signal_handler(SIGQUIT, SIG_IGN, 0);
}

// after fork child process copied the signal handler from parent as well,
//	so it is necessary to change the behavior for child process
// it will perform as default to terminate process like SIG_DFL
// set handling behavior for ctrl-c, as parent process ,
// i do nothing o react to signal, i will waitfor child process
// set handling behavior for ctrl-d, SIGQUIT, which is ignore,
//	nothing happens
void	set_signal_in_exe_child_process(void)
{
	set_signal_handler(SIGINT, SIG_DFL, 0);
	set_signal_handler(SIGQUIT, SIG_DFL, 0);
}
