/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collect_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylang <ylang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 19:06:06 by lilypad           #+#    #+#             */
/*   Updated: 2026/01/21 18:40:43 by ylang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*a function to readline read input from terminal heredoc line by line to terminal
finish read, close the tmp file desciption
*/

int	read_heredoc_lines(int tmp_file_des, const char *delimiter,
		t_shell_context *sh_ctx)
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
		if (write(tmp_file_des, line, ft_strlen(line)) == -1
			|| write(tmp_file_des, "\n", 1) == -1)
		{
			free(line);
			return (EXIT_FAILURE);
		}
		free(line);
	}
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

	raw_delim = node->u_data.redirection.file_path;
	printf("raw_delim is %s \n", raw_delim);
	clean_delim = heredoc_delimiter_strip(raw_delim, NULL, sh_ctx);
	printf("clean_delim is %s\n", clean_delim);
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
	status = read_heredoc_lines(fd, clean_delim, sh_ctx);
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
char	*heredoc_delimiter_strip(const char *raw, bool *quoted,
		t_shell_context *sh_ctx)
{
	char	*out;

	size_t len, i, o, start, end;
	(void)sh_ctx;
	if (quoted)
		*quoted = false;
	if (!raw)
		return (NULL);
	len = ft_strlen(raw);
	start = 0;
	end = len;
	/* strip exactly one matching pair of outer quotes */
	//Should be not only first one \' but also and the  end if there is paring double quote or signle quote 
	if (len >= 2 && (raw[0] == '\'' || raw[0] == '"') && raw[len - 1] == raw[0])
	{
		if (quoted)
			*quoted = true;
		start = 1;
		end = len - 1; /* exclusive */
	}
	out = malloc((end - start) + 1);
	if (!out)
		return (NULL);
	i = start;
	o = 0;
	while (i < end)
	{
		if (raw[i] == '\\' && i + 1 < end)
			i++;
		out[o++] = raw[i++];
	}
	out[o] = '\0';
	return (out);
}
