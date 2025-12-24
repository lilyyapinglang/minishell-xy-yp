#include "../inc/minishell.h"

/*
cat << EOF
hello
^C
minishell$

to turn all cmd << EOF to cmd < /tmp/minishell_heredoc_X.
Handle current heredocs node, and turn it into temp file input,
*/
int	collect_all_heredocs_from_this_node(t_ast *node, t_shell *shell)
{
	int		search_result;
	int		current_temp_files_counts;
	char	*current_file_num_suffix;
	char	*tmp_file_name;
	int		tmp_file_id;

	if (!is_heredoc(node))
	{
		search_result = search_for_heredocs_from_this_node(node, shell);
		return (search_result); // 0 for no such node, 1 mean yes
	}
	// current node has heredoc to colect, we need to collect from readlines
	current_temp_files_counts = ft_lstsize(shell->temp_files);
	current_file_num_suffix = ft_itoa(current_temp_files_counts);
	tmp_file_name = ft_strjoin("/tmp/minishell_heredoc_",
			current_file_num_suffix);
	// open this tmp file to get fd;
	tmp_file_des = open(tmp_file_name, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	// read input to this fd;
	int status = ? ? ?
						// a function to readline read input from terminal heredoc line by line to terminal
						// finish read, close the tmp file desciption
		close(tmp_file_des);
	// cmd < /tmp/minishell_heredoc_X.
	node->u_data.redirection.file = tmp_file_name;
	if (node->u_data.redirection.child)
	{
		search_result = search_for_heredocs_from_this_node(node->u_data.redirection.child,
				shell);
		return (search_result);
	}
	return (status);
}

/*
typedef enum e_token_type
{
	TK_ERROR,
	TK_WORD,
	TK_PIPE, |
	TK_AND, &&
	TK_OR, ||
	TK_REDIRECT_IN,  cat < input.txt
	TK_REDIRECT_OUT,  echo hi > out.txt
	TK_APPEND_OUT, echo hi >> out.txt
	TK_HEREDOC,
(cat << EOF
	hello
	EOF)

	TK_SUBSHELL_OPEN, (
	TK_SUBSHELL_CLOSE )
}					t_token_type;

typedef struct s_ast_redirection
{
	t_token_type	direction;
	struct s_ast	*child;
	char			*file;
}					t_ast_redirection;
*/

/*
echo hi > a > b

REDIR (>)
├── child: REDIR (>)
│   ├── child: COMMAND(echo hi)
│   └── file: "a"
└── file: "b"

*/
int	is_heredoc(t_ast *node, t_shell *shell)
{
	if (node &node->type = AST_REDIRECTION &&node->u_data.redirection = "<<")
		// TK_HEREDOCS
		return (1);
	return (0);
}

int	search_for_heredocs_from_this_node(t_ast *node, t_shell *shell)
{
	if (!node)
		return (0);
	// recursively collect
	else if (is_heredoc(node))
		return (collect_all_heredocs_from_this_node(node, shell));
	// search in ast_redirection child,
	else if (node->type == AST_REDIRECTION)
	{
		if (search_for_heredocs_from_this_node(node->u_data.redirection.child))
			return (1);
		return (0);
	}
	// if it is logical, i need to search both side of $$ ||
	else if (node->type == AST_LOGICAL)
	{
		if (search_for_heredocs_from_this_node(node->u_data.logical.left)
			|| search_for_heredocs_from_this_node(node->u_data.logical.right))
			return (1);
		return (0);
	}
	else if (node->type = AST_PIPELINE)
	{
		if (search_for_heredocs_from_this_node(node->u_data.pipeline.left)
			|| search_for_heredocs_from_this_node(node->u_data.pipeline.right))
			return (1);
		return (0);
	}
	// search in the child
	else if (node->type = AST_SUBSHELL)
	{
		if (search_for_heredocs_from_this_node(node->u_data.subshell.child))
			return (1);
		return (0);
	}
	// node->type = command or node->type = error ,return 0
	return (0);
}

char	*trim_delimiter(char *delimiter, t_shell *shell)
{
	char first_char;

	first_char = *delimiter;
	// 'EOF' "EOF"
	if (first_char == '\'' || | first_char = '\"')
		delimiter = ft_strtrim(delimiter, first_char); // allocated,
														//	need to free
	return (delimiter);
}