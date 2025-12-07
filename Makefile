NAME_PARSING:= parsing
NAME_EXECUTION := execution

CC := cc 
CFLAGS := -Wall -Wextra -Werror -lreadline -g

INC_DIR := inc
SRC_DIR := src
FT_PRINTF_DIR := lib/ft_printf
FT_PRINTF_A = $(FT_PRINTF_DIR)/libftprintf.a

INCS := -I$(INC_DIR)

#PARSING_DIR := $(SRC_DIR)/parsing
EXECUTION_DIR := $(SRC_DIR)/execution

#PARSING_SRC := $(EXECUTION_DIR)/parsing.c 
EXECUTION_SRC := $(EXECUTION_DIR)/execution.c $(EXECUTION_DIR)/builtin_cmd.c

EXECUTION_OBJ := $(EXECUTION_SRC:.c =.o)

#OBJ :=$(addprefix $(BUILD_DIR)/, $(SRC:.c = .o))

# Rules

all : $(NAME_EXECUTION)

#all : $(NAME_PARSING) $(NAME_EXECUTION)

#$(NAME_PARSING) : $(PARSING_DIR) $(FT_PRINTF_A)
#	$(CC) $(CFLAGS) $(INCS) $^ -o $@

$(NAME_EXECUTION) : $(EXECUTION_OBJ) $(FT_PRINTF_A)
	$(CC) $(CFLAGS) $(INCS) $^ -o $@

%.c : %.o
	$(CC) $(FLAGS) $(INCS) -c $< -o $@
$(NAME) : $(FT_PRINTF_A) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ)# -o $@

# ft_printf
$(FT_PRINTF_A):
	$(MAKE) -C $(FT_PRINTF_DIR)

clean: 
	$(RM) $(NAME_EXECUTION)
	$(MAKE) -C $(FT_PRINTF_DIR) clean

fclean: clean
	$(MAKE) -C $(FT_PRINTF_DIR) fclean

re: fclean all

norm: 
	norminette $(INC_DIR) $(SRC_DIR)


.PHONY: all clean fclean re norm
