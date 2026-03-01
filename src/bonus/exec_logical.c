/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_logical.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilypad <lilypad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 21:28:45 by ylang             #+#    #+#             */
/*   Updated: 2026/03/01 11:49:01 by lilypad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*
此时 execute_logical 的策略（概念上）是：
先执行 OR 的 left（也就是 AND 子树），拿到 status_left
如果 status_left == 0：短路成功，不执行 right
否则执行 OR 的 right，返回其 status

*/

int	execute_logical(t_ast *node, t_shell_context *sh_ctx) {
  int left_status;
  int result_status;
  t_ast_logical *logical_node;

  logical_node = &node->u_data.logical;
  left_status = execute(logical_node->left, RUN_IN_SHELL, sh_ctx);
  result_status = left_status;
  if (logical_node->operator== TOKEN_AND && left_status == 0)
    result_status = execute(logical_node->right, RUN_IN_SHELL, sh_ctx);
  else if (logical_node->operator== TOKEN_OR && left_status != 0) {
    result_status = execute(logical_node->right, RUN_IN_SHELL, sh_ctx);
  }
  sh_ctx->last_status = result_status;
  return (result_status);
}
