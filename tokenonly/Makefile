#Makefile only for token

# **************************************************************************** #
#                                 MINISHELL LEXER MAKEFILE                     #
# **************************************************************************** #

# --- Project Setup ---
NAME		= lexer_test
SRC_DIR		= srcs/token
OBJ_DIR		= obj
INCLUDE_DIR	= include
LIBFT_PATH	= libft

SRCS		= add_list.c \
			  extract_word.c \
			  token_test.c \
			  token_utile_all.c \
			  tokenize.c

SRC_FILES	= $(addprefix $(SRC_DIR)/, $(SRCS))

OBJS		= $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRCS))

# --- Compilation Flags ---
CC			= gcc
CFLAGS		= -Wall -Wextra -Werror -g -I$(INCLUDE_DIR) -I$(LIBFT_PATH)/include

# --- Dependencies ---

LIBFT_LIB	= $(LIBFT_PATH)/libft.a

# --- Targets ---

$(NAME): $(OBJS)
	#@echo "Linking $(NAME)..."
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT_LIB) -o $@
	#@echo "Compilation successful. Execute with: ./$(NAME) \"<command>\""

$(LIBFT_LIB):
	@echo "Compiling Libft..."
	@make -C $(LIBFT_PATH)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "Cleaning objects..."
	@rm -rf $(OBJ_DIR)
	@make clean -C $(LIBFT_PATH)

fclean: clean
	@echo "Cleaning executable..."
	@rm -f $(NAME)
	@make fclean -C $(LIBFT_PATH)

re: fclean all

.PHONY: all clean fclean re