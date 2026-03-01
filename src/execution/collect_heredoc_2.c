/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collect_heredoc_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilypad <lilypad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 22:19:31 by ylang             #+#    #+#             */
/*   Updated: 2026/03/01 11:49:01 by lilypad          ###   ########.fr       */
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
