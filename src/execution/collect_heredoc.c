#include "../inc/minishell.h"

/*
cat << EOF
hello
^C
minishell$

to turn all cmd << EOF to cmd < /tmp/minishell_heredoc_X.
Handle current heredocs node, and turn it into temp file input,
*/

int	collect_all_heredocs_from_this_node(t_ast *node,
		t_shell_context *shell_context)
{
	int		search_result;
	int		current_temp_files_counts;
	char	*current_file_num_suffix;
	char	*tmp_file_name;
	int		tmp_file_des;
	int		status;
	char	*raw_delimiter;
	char	*clean_delimiter;

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
	raw_delimiter = node->u_data.redirection.right_hand_side;
	clean_delimiter = get_clean_heredoc_delimiter(raw_delimiter, shell);
	status = collect_heredocs_from_terminal(tmp_file_des, clean_delimiter,
			shell);
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
int	is_heredoc(t_ast *node, t_shell_context *shell_conetext)
{
	if (node &node->type = AST_REDIRECTION &&node->u_data.redirection = "<<")
		// TK_HEREDOCS
		return (1);
	return (0);
}

int	search_for_heredocs_from_this_node(t_ast *node,
		t_shell_context *shell_conetext)
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

/*return a newlly allocated string,
	must be freed by caller or
	here
	// right_hand_side is raw string
	// need to remove quote and escape\
//'EOF' "EOF"  E"OF" \EOF E\"OF
*/
char	*get_clean_heredoc_delimiter(const char *raw_delimiter,
		t_shell_context *shell_conetext)
{
	size_t	i;
	size_t	o;
	bool	in_quote;
	char	quote_mark;
	char	*cleaned_delimiter;

	i = 0;
	o = 0;
	in_quote = false;
	quote_mark = 0;
	if (!raw_delimiter)
		return (NULL);
	// output length will never exceed raw length
	// record it as a pointer to the list and free togethwe with others
	cleaned_delimiter = malloc((ft_strlen(raw_delimiter) + 1) * sizeof(char));
	while (raw_delimiter[i])
	{
		if (raw_delimiter[i] == '\\' && raw_delimiter[i + 1])
		{
			// skip, record  to next one
			i++;
			cleaned_result[o++] = raw_delimiter[i++];
			continue ;
		}
		if (raw_delimiter[i] == '\'' || raw_delimiter[i] == '\"')
		{
			if (in_quote = false)
			{ // current ' and " is not in quote , so need to igtnore,
				//	itself turns falt quote to true
				in_quote = true;
				quote_mark = raw_delimiter[i]; // q could be quote ' or quote "
			}
			else if (raw_delimiter[i] == quote_mark)
				in_quote = false; // after the second quote_mark,
			// we are not in quote anymore i++;
			continue ;
		}
		cleaned_delimiter[o++] = raw_delimiter[i++];
	}
	cleaned_delimiter[o] = '\0';
	// defensive
	if (in_quote == true)
		error("heredoc", "unexpected unclosed quote in delimiter", EXIT_FAILURE,
			shell);
	return (cleaned_delimiter);
}

char	*trim_delimiter(char *delimiter, t_shell_context *shell_conetext)
{
	char	first_char;

	first_char = *delimiter;
	// 'EOF' "EOF"
	if (first_char == '\'' || first_char = '\"')
		delimiter = ft_strtrim(delimiter, &first_char); // allocated,
														//	need to free
	return (delimiter);
}

/*a function to readline read input from terminal heredoc line by line to terminal
finish read, close the tmp file desciption
*/

int	collect_heredocs_from_terminal(int tmp_file_des, char *delimiter,
		t_shell_context *shell_conetext)
{
	char	*user_input;

	user_input = NULL;
	g_latest_signal_status = 0;
	while (true)
	{
		// i need to know if it is main prompt or if is heredoc prompt,
		// if (isatty(fileno(stdin)))
		{
			rl_replace_line("", 0);
			rl_on_new_line();
			handle_signal_in_heredoc_prompt_mode();
			user_input = readline("> ");
			handle_signal_in_exe_main_process();
		}
		if (g_latest_signal_status == SIGINT)
			return (130);
		if (!user_input)
		{
			errno("error");
			return (0);
		}
		if (ft_strncmp(user_input, delimiter) == 0)
		{
			free(user_input);
			return (0);
		}
		write(user_input, tmp_file_des, shell);
		write("\n", tmp_file_des, shell);
		free(user_input);
	}
	return (0);
}
