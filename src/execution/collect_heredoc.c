/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collect_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylang <ylang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 19:06:06 by lilypad           #+#    #+#             */
/*   Updated: 2026/01/24 17:27:10 by ylang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*a function to readline read input from terminal heredoc line by line to terminal
finish read, close the tmp file desciption
*/

int	read_heredoc_lines(int tmp_file_des, const char *delimiter,
		t_shell_context *sh_ctx, bool is_quoted)
{
	char	*line;

	(void)sh_ctx;
	g_latest_signal_status = 0;
	while (1)
	{
		// i need to know if it is main prompt or if is heredoc prompt,
		line = prompt_listener(HEREDOC_PROMPT);
		/* Ctrl-C: abort heredoc AND cancel whole command line */
		/*  FIRST: did Ctrl-C happen? */
		if (g_latest_signal_status == SIGINT)
		{
			if (line)
				free(line);
			return (130);
		}
		/* EOF: stop heredoc early (optional warning) */
		/*  SECOND: did we hit EOF (Ctrl-D)? */
		if (!line)
		{
			// errno("error collecting ");
			return (EXIT_SUCCESS);
		}
		/* THIRD: delimiter match */
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			return (EXIT_SUCCESS);
		}
		/* LAST: write content */
		// how to integrate expander logic here ?
		if (is_quoted)
		{
			if (write(tmp_file_des, line, ft_strlen(line)) == -1
				|| write(tmp_file_des, "\n", 1) == -1)
			{
				free(line);
				return (EXIT_FAILURE);
			}
			free(line);
		}
		else // need to check for expand
		{
			// expande variables, and then write
			// TODO
			if (write(tmp_file_des, line, ft_strlen(line)) == -1
				|| write(tmp_file_des, "\n", 1) == -1)
			{
				free(line);
				return (EXIT_FAILURE);
			}
			free(line);
		}
	}
}

/*
(cat << EOF
	hello
	EOF)
echo hi > a > b

REDIR (>)
├── child: REDIR (>)
│   ├── child: COMMAND(echo hi)
│   └── file: "a"
└── file: "b"
*/
/*return a newlly allocated string,
	must be freed by caller or
	here
	// right_hand_side is raw string
	// need to remove quote and escape\
//'EOF' "EOF"  E"OF" \EOF E\"OF
*/

char	*heredoc_delimiter_strip(const char *raw, bool *is_quoted,
		t_shell_context *sh_ctx)
{
	t_quote_state	mode;
	char			*ptr;
	char			*result;
	char			*result_ptr;

	(void)sh_ctx;
	if (!raw)
		return (NULL);
	if (is_quoted)
		*is_quoted = false;
	mode = QUOTE_NONE;
	ptr = (char *)raw;
	result = malloc(ft_strlen(raw) + 1);
	if (!result)
		return (NULL);
	result_ptr = result;
	while (*ptr)
	{
		if (mode == QUOTE_NONE)
		{
			if (*ptr == '\'')
			{
				mode = QUOTE_SINGLE;
				if (is_quoted)
					*is_quoted = true;
			}
			else if (*ptr == '"')
			{
				mode = QUOTE_DOUBLE;
				if (is_quoted)
					*is_quoted = true;
			}
			else
				*result++ = *ptr;
		}
		else if (mode == QUOTE_SINGLE)
		{
			if (*ptr == '\'')
				mode = QUOTE_NONE;
			else
				*result++ = *ptr;
		}
		else if (mode == QUOTE_DOUBLE)
		{
			if (*ptr == '"')
				mode = QUOTE_NONE;
			else
				*result++ = *ptr;
		}
		ptr++;
	}
	if (mode != QUOTE_NONE)
	{
		free(result_ptr);
		print_msg("heredoc", raw, "syntax error (unclosed quote)\n");
		return (NULL);
	}
	*result = '\0';
	return (result_ptr);
}
/*
cat << EOF
hello
^C
minishell$

to turn all cmd << EOF to cmd < /tmp/minishell_heredoc_X.
Handle current heredocs node, and turn it into temp file input,
*/

static int	collect_one_heredoc(t_ast *node, t_shell_context *sh_ctx)
{
	int		fd;
	int		status;
	char	*tmp_name;
	char	*suffix;
	t_list	*lstnode;
	char	*raw_delim;
	char	*clean_delim;
	bool	is_quoted;

	raw_delim = node->u_data.redirection.file_path;
	clean_delim = heredoc_delimiter_strip(raw_delim, &is_quoted, sh_ctx);
	if (!clean_delim)
		return (EXIT_FAILURE);
	/* build /tmp/minishell_heredoc_N */
	// TODO: ft_itoa_s?
	suffix = ft_itoa(ft_lstsize(sh_ctx->temporary_files));
	if (!suffix)
		return (free(clean_delim), 1);
	// TODO: ft_strjoin_s?
	tmp_name = ft_strjoin("/tmp/minishell_heredoc_", suffix);
	free(suffix);
	if (!tmp_name)
		return (free(clean_delim), 1);
	// TODO: open_s?
	fd = open(tmp_name, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd == -1)
		return (free(clean_delim), free(tmp_name), 1);
	/* register tmp_name for later unlink+free */
	lstnode = ft_lstnew(tmp_name);
	if (!lstnode)
	{
		close(fd);
		unlink(tmp_name);
		free(tmp_name);
		free(clean_delim);
		return (1);
	}
	ft_lstadd_back(&sh_ctx->temporary_files, lstnode);
	status = read_heredoc_lines(fd, clean_delim, sh_ctx, is_quoted);
	free(clean_delim);
	close(fd);
	if (status != EXIT_SUCCESS)
		return (status);
	/* success: replace delimiter with temp file path */
	node->u_data.redirection.file_path = tmp_name;
	return (EXIT_SUCCESS);
}

/*scan from root to collect heredocs */
int	collect_all_heredocs(t_ast *node, t_shell_context *sh_ctx)
{
	int	status;

	if (!node)
		return (EXIT_SUCCESS);
	if (node->type == AST_SYNTAX_ERROR)
		return (EXIT_FAILURE);
	if (node->type == AST_PIPELINE)
	{
		status = collect_all_heredocs(node->u_data.pipeline.left, sh_ctx);
		if (status != EXIT_SUCCESS)
			return (status);
		return (collect_all_heredocs(node->u_data.pipeline.right, sh_ctx));
	}
	if (node->type == AST_LOGICAL)
	{
		status = collect_all_heredocs(node->u_data.logical.left, sh_ctx);
		if (status != EXIT_SUCCESS)
			return (status);
		return (collect_all_heredocs(node->u_data.logical.right, sh_ctx));
	}
	if (node->type == AST_SUBSHELL)
		return (collect_all_heredocs(node->u_data.subshell.child, sh_ctx));
	if (node->type == AST_REDIRECTION)
	{
		/* collect inside first (keeps left-to-right order for nested redirs) */
		status = collect_all_heredocs(node->u_data.redirection.exe_child,
				sh_ctx);
		if (status != EXIT_SUCCESS)
			return (status);
		if (node->u_data.redirection.redir_type == REDIR_HEREDOC)
			return (collect_one_heredoc(node, sh_ctx));
		return (EXIT_SUCCESS);
	}
	/* AST_COMMAND: nothing to collect */
	return (EXIT_SUCCESS);
}
