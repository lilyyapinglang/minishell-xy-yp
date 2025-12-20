/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <xuewang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 20:15:19 by xuewang           #+#    #+#             */
/*   Updated: 2025/12/20 20:27:17 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/token.h"
#include "../include/parse.h"

t_redir *redir_create(t_token_type type, char *target)
{
    t_redir *redir;

    redir = safe_malloc(sizeof(t_redir));
    if (!redir)
        return (NULL);
    redir->type = type;
    redir->target = ft_strdup(target);
    if (!redir->target)
    {
        free(redir);
        return (NULL);
    }
    redir->heredoc_fd = -1;
    redir->next = NULL;
    return (redir);
}

void redir_add_back(t_redir **list, t_redir *new)
{
    t_redir *last;

    if (!list || !new)
        return;
    if (!*list)
    {
        *list = new;
        return;
    }
    last = *list;
    while (last->next)
        last = last->next;
    last->next = new;
}

int parse_redir(t_parser *parser, t_cmd *cmd)
{
    t_token_type type;
    t_redir *redir;

    if (!parser || !cmd || !parser->current)
        return (0);

    type = parser->current->type;
    advance(parser);

    if (!parser->current)
    {
        syntax_error("newline");
        parser->error = 1;
        return (0);
    }
    if (parser->current->type != TOKEN_WORD)
    {
        syntax_error(parser->current->value);
        parser->error = 1;
        return (0);
    }

    redir = redir_create(type, parser->current->value);
    if (!redir)
        return (0);

    redir_add_back(&cmd->redirects, redir);
    advance(parser);
    return (1);
}
