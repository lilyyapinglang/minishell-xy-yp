/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <xuewang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 11:59:43 by xuewang           #+#    #+#             */
/*   Updated: 2025/12/03 16:06:49 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
#define TOKEN_H

#include <readline/readline.h>
#include <readline/history.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "../libft/libft.h"

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_SEMICOLON,
	TOKEN_LESS,
	TOKEN_GREAT,
	TOKEN_DLESS,
	TOKEN_DGREAT,
	TOKEN_EOF
} t_token_type;

typedef enum e_quote_state
{
	QUOTE_NONE, // not in the quote status
	QUOTE_SINGLE,
	QUOTE_DOUBLE
} t_quote_state;

/*chain of the tokens */
/*one direct is enough*/

typedef struct s_tokenlist
{
	t_token_type type;
	char *value;
	struct s_tokenlist *next;
} t_tokenlist;

/*struct to save the current input status*/
typedef struct s_tokenize
{
	char *input;
	int pos;
	t_quote_state quote; 
	t_tokenlist *tokens;
} t_tokenize;

char *extract_word(t_tokenize *lex);

t_tokenlist *lexer_tokenize(char *input);

/*utile*/

int is_operator_char(char c);
t_token_type get_operator_type(char *str, int *len);
void print_quote_error(t_quote_state quote);
int handle_quotes(t_tokenize *lex, char quote_char);
void *safe_malloc(size_t size);

int ft_isspace(char c);

/*list*/

t_tokenlist *token_create(t_token_type type, char *value);
void token_add(t_tokenlist **list, t_tokenlist *new);
void token_list_free(t_tokenlist *tokens);

/*test*/
const char *get_token_type_name(t_token_type type);
void run_test(char *input);
void display_tokens(t_tokenlist *tokens);
int main(int argc, char **argv);
//int main(void);
#endif
