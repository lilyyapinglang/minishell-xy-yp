#include "minishell.h"

/*
在 minishell 里，错误只有 3 个维度，
维度 1：错误是否致命（fatal vs non-fatal）
fatal：打印 → 清理 → 退出当前进程
non-fatal：打印 → 返回错误码，让上层决定
这是最重要的分界线，必须在 API 层面体现出来。
维度 2：错误来源
errno 型（open / fork / dup2 / execve / unlink …）
语义/语法型（syntax error、ambiguous redirect、not a valid identifier …）
这决定 message 是 strerror(errno) 还是自定义字符串。
维度 3：bash 风格的「主语」
bash 的错误几乎永远是这个结构：
bash: [command:] [argument:] message
例如：
bash: cd: /nope: No such file or directory
bash: infile: No such file or directory
bash: syntax error near unexpected token '|'
我们需要 command 和 argument 两个可选字段。
*/
// 2 types of print, print and return exit failture to upper level

#ifndef SHELL_NAME
# define SHELL_NAME "minishell"
#endif

static void	put_raw(const char *str)
{
	if (str && *str)
		ft_putstr_fd(str, STDERR_FILENO);
}
static void	put_segment(const char *str)
{
	if (str && *str)
	{
		put_raw(str);
		put_raw(": ");
	}
}

/*
** Bash-like format:
** minishell : [cmd: ] [arg:] message \n
*/
static void	print_error(const char *cmd, const char *arg, const char *msg)
{
	put_raw(SHELL_NAME);
	put_raw(": ");
	put_segment(cmd);
	put_segment(arg);
	put_raw(msg ? msg : "");
	put_raw("\n");
}

/*
** minishell: [cmd: ] [arg: ] message\n
*/
static void	print_error(const char *cmd, const char *arg, const char *msg)
{
	put_raw(SHELL_NAME);
	put_raw(": ");
	put_segment(cmd);
	put_segment(arg);
	put_raw(msg ? msg : "");
	put_raw("\n");
}

/* ---------------- public: print only (optional) ---------------- */

void	print_errno(const char *cmd, const char *arg, int errnum)
{
	print_error(cmd, arg, strerror(errnum));
}

void	print_msg(const char *cmd, const char *arg, const char *msg)
{
	print_error(cmd, arg, msg);
}

/* ---------------- public: print + return(code) ---------------- */

int	print_errno_n_return(int code, const char *cmd, const char *arg, int errnum)
{
	print_error(cmd, arg, strerror(errnum));
	return (code);
}

int	print_msg_n_return(int code, const char *cmd, const char *arg, const char *msg)
{
	print_error(cmd, arg, msg);
	return (code);
}

/* ---------------- public: fatal in shell (rare) ---------------- */

void	fatal_errno_shell_quit(t_shell_context *sh_ctx, int code,
		const char *cmd, const char *arg, int errnum)
{
	print_error(cmd, arg, strerror(errnum));
	quit_shell(code, sh_ctx);
}

void	fatal_msg_shell_quit(t_shell_context *sh_ctx, int code,
		const char *cmd, const char *arg, const char *msg)
{
	print_error(cmd, arg, msg);
	quit_shell(code, sh_ctx);
}