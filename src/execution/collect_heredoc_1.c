/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collect_heredoc_1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilypad <lilypad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 19:06:06 by lilypad           #+#    #+#             */
/*   Updated: 2026/03/01 11:43:16 by lilypad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
a function to readline read input from terminal
heredoc line by line to terminal
finish read, close the tmp file desciption
*/
// i need to know if it is main prompt or if is heredoc prompt,
/* Ctrl-C: abort heredoc AND cancel whole command line */
/* FIRST: did Ctrl-C happen? */
/* EOF: stop heredoc early (optional warning) */
/*  SECOND: did we hit EOF (Ctrl-D)? */
// errno("error collecting ");
/* THIRD: delimiter match */
/* LAST: write content */
// how to integrate expander logic here ?
// need to check for expand

// expande variables, and then write
// xueyan changed write(tmp_file_des, line,
// ft_strlen(line)) to write(tmp_file_des, expanded,
// ft_strlen(expanded))
static int	write_heredoc_line(int fd, const char *line)
{
	if (write(fd, line, ft_strlen(line)) == -1)
		return (EXIT_FAILURE);
	if (write(fd, "\n", 1) == -1)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

/* expand line variables and write to fd */
static int	write_heredoc_expanded(int fd, const char *line,
		t_shell_context *sh)
{
	char	*expanded;
	int		ret;

	expanded = heredoc_expand_line(line, sh);
	if (!expanded)
		return (EXIT_FAILURE);
	ret = write_heredoc_line(fd, expanded);
	free(expanded);
	return (ret);
}

/* read heredoc lines from terminal until delimiter */

int	read_heredoc_lines(int fd, const char *delimiter, t_shell_context *sh,
		bool is_quoted)
{
	char	*line;
	int		ret;

	g_latest_signal_status = 0;
	while (1)
	{
		line = prompt_listener(HEREDOC_PROMPT);
		if (g_latest_signal_status == SIGINT)
			return (130);
		if (!line)
			return (EXIT_SUCCESS);
		if (ft_strcmp(line, delimiter) == 0)
			return (free(line), EXIT_SUCCESS);
		if (is_quoted)
			ret = write_heredoc_line(fd, line);
		else
			ret = write_heredoc_expanded(fd, line, sh);
		free(line);
		if (ret != EXIT_SUCCESS)
			return (ret);
	}
}

int	collect_all_heredocs_from_redir_node(t_shell_context *sh_ctx, t_ast *node)
{
	int	status;

	status = collect_all_heredocs(node->u_data.redirection.exe_child, sh_ctx);
	if (status != EXIT_SUCCESS)
		return (status);
	if (node->u_data.redirection.redir_type == REDIR_HEREDOC)
		return (collect_one_heredoc(node, sh_ctx));
	return (EXIT_SUCCESS);
}
