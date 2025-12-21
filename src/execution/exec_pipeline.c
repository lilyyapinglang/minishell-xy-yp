#include "../inc/minishell.h"

/*
pipeline = build_cmd_list(node, sh)
把 echo "$X" 和 grep foo >out1 变成线性 list：[echo_node, redir(grep>out1)_node]
execute_pipeline_list(pipeline, sh)
循环：每个 stage 都 fork 子进程执行，并用 pipe 串接

*/

t_ast_pipeline	*build_cmd_list(t_ast *pipeline_node, t_shell *shell)
{
	t_ast_pipeline	*pipeline_list;

	// 把树压平到一个CMD list
}
int	execute_pipeline(t_ast *pipeline_node, t_shell shell)
{
	t_ast_pipeline	*pipeline;

	pipeline = build_cmd_list(pipeline_node, shell);
	execute_pipeline_list(pipeline, shell);
}
