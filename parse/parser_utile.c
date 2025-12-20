/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utile.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <xuewang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 21:39:07 by xuewang           #+#    #+#             */
/*   Updated: 2025/12/20 21:34:36 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/token.h"
#include "../include/parse.h"

/* detect stop */

int	is_stop_token(t_token_type type)
{
	if (type == TOKEN_PIPE)
		return (1);
	if (type == TOKEN_AND)
		return (1);
	if (type == TOKEN_OR)
		return (1);
	if (type == TOKEN_RPAREN)
		return (1);
	if (type == TOKEN_EOF)
		return (1);
	return (0);
}

/* [CHANGED] AST 时代：cmd 仍然是叶子节点内容（argv/redirects），next 不再用于 pipeline */
t_cmd	*cmd_create(void)
{
	t_cmd	*cmd;

	cmd = safe_malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->argv = NULL;
	cmd->redirects = NULL;

	/* next 在 AST 时代通常不用来串 pipeline，但保留字段不影响 */
	cmd->next = NULL;

	/* 如果你的 executor 还用 pipe_fd 做执行期管理，可以保留初始化 */
	cmd->pipe_fd[0] = -1;
	cmd->pipe_fd[1] = -1;
	return (cmd);
}

void	cmd_free_one(t_cmd *cmd)
{
	if (!cmd)
		return ;
	cleanup_cmd_fds(cmd);
	if (cmd->argv)
		ft_free_array(cmd->argv);
	if (cmd->redirects)
		redir_list_free(cmd->redirects);
	free(cmd);
}
