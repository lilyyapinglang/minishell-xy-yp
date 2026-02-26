/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collect_heredoc_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilypad <lilypad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 22:19:31 by ylang             #+#    #+#             */
/*   Updated: 2026/02/26 13:49:46 by lilypad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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
/* update quote state for heredoc delimiter parsing */
// static void	update_quote_state(t_quote_state *mode, char c, bool *is_quoted)
// {
// 	if (*mode == QUOTE_NONE)
// 	{
// 		if (c == '\'')
// 		{
// 			*mode = QUOTE_SINGLE;
// 			if (is_quoted)
// 				*is_quoted = true;
// 		}
// 		else if (c == '"')
// 		{
// 			*mode = QUOTE_DOUBLE;
// 			if (is_quoted)
// 				*is_quoted = true;
// 		}
// 	}
// 	else if ((*mode == QUOTE_SINGLE && c == '\'') || (*mode == QUOTE_DOUBLE
// 			&& c == '"'))
// 		*mode = QUOTE_NONE;
// }

/* determine if char should be appended to cleaned delimiter */
// static bool	should_append(t_quote_state mode, char c)
// {
// 	return (mode == QUOTE_NONE || (mode == QUOTE_SINGLE && c != '\'')
// 			|| (mode == QUOTE_DOUBLE && c != '"'));
// }

/* allocate buffer for cleaned delimiter using tracked allocation */
// static char	*alloc_delim_buffer(const char *raw, t_shell_context *sh)
// {
// 	return (s_alloc(ft_calloc(ft_strlen(raw) + 1, sizeof(char)),
// 			ALLOC_UNTRACKED, sh));
// }

/* strip quotes from heredoc delimiter, sets is_quoted if quotes found */
// char	*heredoc_delimiter_strip(const char *raw, bool *is_quoted,
// 		t_shell_context *sh_ctx)
// {
// 	t_quote_state	mode;
// 	char			*ptr;
// 	char			*result;
// 	char			*res_start;

// 	if (!raw)
// 		return (NULL);
// 	if (is_quoted)
// 		*is_quoted = false;
// 	mode = QUOTE_NONE;
// 	ptr = (char *)raw;
// 	result = alloc_delim_buffer(raw, sh_ctx);
// 	res_start = result;
// 	while (*ptr)
// 	{
// 		update_quote_state(&mode, *ptr, is_quoted);
// 		if (should_append(mode, *ptr))
// 			*result++ = *ptr;
// 		ptr++;
// 	}
// 	if (mode != QUOTE_NONE)
// 		return (free(res_start), print_msg("heredoc", raw,
// 				"syntax error (unclosed quote)\n"), NULL);
// 	*result = '\0';
// 	return (res_start);
// }

char	*update_result_loc(const char *raw, char *result, t_quote_state *mode,
		bool *is_quoted)
{
	char	*ptr;

	ptr = (char *)raw;
	while (*ptr)
	{
		if (*mode == QUOTE_NONE)
		{
			if (*ptr == '\'')
			{
				*mode = QUOTE_SINGLE;
				if (*is_quoted == false)
					*is_quoted = true;
			}
			else if (*ptr == '"')
			{
				*mode = QUOTE_DOUBLE;
				if (*is_quoted == false)
					*is_quoted = true;
			}
			else
				*result++ = *ptr;
		}
		else if (*mode == QUOTE_SINGLE)
		{
			if (*ptr == '\'')
				*mode = QUOTE_NONE;
			else
				*result++ = *ptr;
		}
		else if (*mode == QUOTE_DOUBLE)
		{
			if (*ptr == '"')
				*mode = QUOTE_NONE;
			else
				*result++ = *ptr;
		}
		ptr++;
	}
	return (result);
}
char	*heredoc_delimiter_strip(const char *raw, bool *is_quoted,
		t_shell_context *sh_ctx)
{
	t_quote_state	mode;
	char			*result;
	char			*result_ptr;

	(void)sh_ctx;
	if (!raw)
		return (NULL);
	if (is_quoted)
		*is_quoted = false;
	mode = QUOTE_NONE;
	result = malloc(ft_strlen(raw) + 1);
	if (!result)
		return (NULL);
	result_ptr = result;
	result = update_result_loc(raw, result, &mode, is_quoted);
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
/* build /tmp/minishell_heredoc_N */
// todoooo: open_s?
/* register tmp_name for later unlink+free */
/* success: replace delimiter with temp file path */

// NULL on error

/* helper: clean delimiter from node */
static char	*get_clean_delim(t_ast *node, t_shell_context *sh, bool *is_quoted)
{
	char	*raw;
	char	*clean;

	raw = node->u_data.redirection.file_path;
	clean = heredoc_delimiter_strip(raw, is_quoted, sh);
	return (clean);
}

/* helper: create temporary heredoc file path and register it */
static char	*create_tmp_heredoc(t_shell_context *sh)
{
	char	*suffix;
	char	*tmp_name;

	suffix = s_alloc(ft_itoa(ft_lstsize(sh->temporary_files)), ALLOC_UNTRACKED,
			sh);
	tmp_name = strjoin_s("/tmp/minishell_heredoc_", suffix, ALLOC_UNTRACKED,
			sh);
	free(suffix);
	lst_add_front_s(tmp_name, &sh->temporary_files, ALLOC_UNTRACKED, sh);
	return (tmp_name);
}

int	collect_one_heredoc(t_ast *node, t_shell_context *sh)
{
	int		fd;
	int		status;
	char	*clean_delim;
	bool	is_quoted;
	char	*tmp_name;

	clean_delim = get_clean_delim(node, sh, &is_quoted);
	if (!clean_delim)
		return (EXIT_FAILURE);
	tmp_name = create_tmp_heredoc(sh);
	fd = open_s(tmp_name, O_WRONLY | O_CREAT | O_TRUNC, 0600, sh);
	status = read_heredoc_lines(fd, clean_delim, sh, is_quoted);
	close_s(fd, sh);
	free(clean_delim);
	if (status == 130)
		return (130);
	if (status != EXIT_SUCCESS)
		return (status);
	node->u_data.redirection.file_path = tmp_name;
	return (EXIT_SUCCESS);
}

/*scan from root to collect heredocs */
/* collect inside first (keeps left-to-right order for nested redirs) */
/* AST_COMMAND: nothing to collect */

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
		return (collect_all_heredocs_from_redir_node(sh_ctx, node));
	return (EXIT_SUCCESS);
}
