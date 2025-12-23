#include "../inc/minishell.h"

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
volatile sig_atomic_t	g_lastest_signal_status = 0;

void	set_signal_handler(int sig_num, void (*signal_handler)(int))
{
	struct sigaction	sig_act;

	sigemptyset(&sig_act.sa_mask);
	sig_act.sa_flags = SA_SIGINFO; // or 0 ?
	sig_act.handler = signal_handler;
	sigaction(sig_num, &sig_act, NULL);
}

void	handle_sigint_in_prompt_mode(int sig_num)
{
	// record that sigint happened
	g_lastest_signal_status = sig_num;
	// change to new line because it's ugly to start new prompt on the same line
	// write is signal safe
	if (write(1, "\n", 1) == -1)
		return ;
	// drop what users has entered into buffer of readline
	// clear current readline buffer, not adding to history
	rl_replace_line("", 0);
	// fix readline inernal status, location of cursur, line start info,
		need to redisplay prompt or not
	rl_on_new_line();
	// redraw clean prompt
	rl_redisplay();
}

void	handle_sigint_in_heredoc_mode(int sig_num)
{
}

void	handle_signal_in_prompt_mode(void)
{
	// set handling behavior for ctrl-c, SIGINT
	// i can not kill shell, i need to clear line and return to prompt
	set_signal_handler(SIGINT, handle_sigint_in_prompt_mode);
	// set handling behavior for ctrl-d, SIGQUIT, which is ignore,
	//	nothing happens
	set_signal_handler(SIGQUIT, SIG_IGN);
}

void	handle_signal_in_heredoc_prompt_mode(void)
{
	// set handling behavior for ctrl-c, SIGINT
	set_signal_handler(SIGINT, handle_sigint_in_heredoc_mode);
	// set handling behavior for ctrl-d, SIGQUIT, which is ignore,
	//	nothing happens
	set_signal_handler(SIGQUIT, SIG_IGN);
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

void	handle_signal_in_exe_main_process(void)
{
	// set handling behavior for ctrl-c, as parent process ,
	// i do nothing o react to signal, i will waitfor child process
	set_signal_handler(SIGINT, SIG_IGN);
	// set handling behavior for ctrl-d, SIGQUIT, which is ignore,
	//	nothing happens
	set_signal_handler(SIGQUIT, SIG_IGN);
}

// after fork child process copied the signal handler from parent as well,
//	so it is necessary to change the behavior for child process
// it will perform as default to terminate process like SIG_DFL
void	handle_signal_in_exe_child_process(void)
{
	// set handling behavior for ctrl-c, as parent process ,
	// i do nothing o react to signal, i will waitfor child process
	set_signal_handler(SIGINT, SIG_DFL);
	// set handling behavior for ctrl-d, SIGQUIT, which is ignore,
	//	nothing happens
	set_signal_handler(SIGQUIT, SIG_DFL);
}