# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: xuewang <xuewang@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/30 16:06:25 by xuewang           #+#    #+#              #
#    Updated: 2025/12/30 19:32:41 by xuewang          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= minishell

CC			= cc
CFLAGS		= -Wall -Wextra -Werror
INCLUDES	= -I./include
LIBS		= -lreadline

GNL_DIR     = get_next_line
GNL_SRCS    = $(GNL_DIR)/get_next_line.c \
              $(GNL_DIR)/get_next_line_utils.c
GNL_OBJS    = $(GNL_SRCS:.c=.o)

LIBFT_DIR := libft
LIBFT     := $(LIBFT_DIR)/libft.a
LIBFT_INC := -I $(LIBFT_DIR)/include

INC := -I include $(LIBFT_INC)

# Source files organized by module
SRCS_MAIN	= srcs/main.c \


SRCS_LEXER	= srcs/lexer/lexer.c \
			  srcs/lexer/lexer_utile.c \
			  srcs/lexer/lexer_single_token.c \
			  srcs/lexer/lexer_test.c \

SRCS_PARSER	= srcs/parser/parser.c \
			  srcs/parser/parser_utile.c \
			  srcs/parser/parser_tk_type.c \
			  srcs/parser/parser_sub.c \
			  srcs/parser/parser_redir.c \
			  srcs/parser/parser_redir_next.c \
			  srcs/parser/parser_build_node.c \
			  srcs/parser/parser_test.c \
			  srcs/parser/parser_basic_act.c

SRCS_EXPAND	= srcs/expander/expander.c \
			  srcs/expander/expander_utile.c \
			  srcs/expander/expander_var.c \
			  srcs/expander/expander_env.c \
			  srcs/expander/expander_single_argv.c 

SRCS_SAFE	= srcs/safe_functions/error.c \
			  srcs/safe_functions/libft_list.c \
			  srcs/safe_functions/quitshell.c \
			  srcs/safe_functions/safe_alloc.c \
			  srcs/safe_functions/safe_list.c \
			  
#real all version
#SRCS        = $(SRCS_TEST_MAIN) $(SRCS_LEXER) $(SRCS_PARSER) $(SRCS_EXPAND)
SRCS		= $(SRCS_LEXER) $(SRCS_SAFE) $(SRCS_PARSER)
OBJS		= $(SRCS:.c=.o)

# Colors for output
GREEN		= \033[0;32m
YELLOW		= \033[0;33m
RESET		= \033[0m

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@echo "$(YELLOW)Linking $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME) $(LIBS)
	@echo "$(GREEN)$(NAME) created successfully!$(RESET)"

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

%.o: %.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

val: $(NAME)
	@if ! [ -f "ignore.supp" ]; then make ignore; fi
	@valgrind --suppressions=$$(pwd)/ignore.supp --leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes --track-fds=yes -s ./$(NAME)

clean:
	@echo "$(YELLOW)Removing object files...$(RESET)"
	@rm -f $(OBJS)
	@echo "$(GREEN)Clean complete!$(RESET)"

fclean: clean
	@echo "$(YELLOW)Removing $(NAME)...$(RESET)"
	@rm -f $(NAME)
	@echo "$(GREEN)Full clean complete!$(RESET)"

re: fclean all

.PHONY: all clean fclean re
