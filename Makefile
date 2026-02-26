NAME := minishell

CC := cc
CFLAGS := -Wall -Wextra -Werror
CPPFLAGS := -Iinc -Ilib/ft_printf -Ilib/libft -Ilib/get_next_line
LDFLAGS :=
LDLIBS :=


# ------------------ Readline (Homebrew macOS) ------------------
READLINE_PREFIX := $(shell brew --prefix readline 2>/dev/null)
ifeq ($(READLINE_PREFIX),)
  READLINE_INC :=
  READLINE_LIB :=
  READLINE_LDLIBS :=
else
  READLINE_INC := -I$(READLINE_PREFIX)/include
  READLINE_LIB := -L$(READLINE_PREFIX)/lib
  READLINE_LDLIBS := -lreadline -lhistory
endif
CPPFLAGS += $(READLINE_INC)
LDFLAGS  += $(READLINE_LIB)
$(NAME): LDLIBS   += $(READLINE_LDLIBS)


# ------------------ Readline (Linux/WSL fallback) ------------------
# If brew readline not found, assume system readline
ifeq ($(READLINE_PREFIX),)
  LDLIBS += -lreadline -lhistory -lncurses
endif


# ------------------ Build dirs ------------------
BUILD_DIR := build
OBJ_DIR   := $(BUILD_DIR)/obj
DEP_DIR   := $(BUILD_DIR)/dep
TEST_DIR  := $(BUILD_DIR)/tests

# ------------------ ft_printf ------------------
FTPRINTF_A := $(BUILD_DIR)/libftprintf.a

FTPRINTF_SRCS := \
  lib/libft/ft_atoi.c \
  lib/libft/ft_bzero.c \
  lib/libft/ft_calloc.c \
  lib/libft/ft_isalnum.c \
  lib/libft/ft_isalpha.c \
  lib/libft/ft_isascii.c \
  lib/libft/ft_isdigit.c \
  lib/libft/ft_isprint.c \
  lib/libft/ft_itoa.c \
  lib/libft/ft_memchr.c \
  lib/libft/ft_memcmp.c \
  lib/libft/ft_memcpy.c \
  lib/libft/ft_memmove.c \
  lib/libft/ft_memset.c \
  lib/libft/ft_putchar_fd.c \
  lib/libft/ft_putendl_fd.c \
  lib/libft/ft_putnbr_fd.c \
  lib/libft/ft_putstr_fd.c \
  lib/libft/ft_split.c \
  lib/libft/ft_strchr.c \
  lib/libft/ft_strdup.c \
  lib/libft/ft_striteri.c \
  lib/libft/ft_strjoin.c \
  lib/libft/ft_strlcat.c \
  lib/libft/ft_strlcpy.c \
  lib/libft/ft_strlen.c \
  lib/libft/ft_strmapi.c \
  lib/libft/ft_strncmp.c \
  lib/libft/ft_strnstr.c \
  lib/libft/ft_strrchr.c \
  lib/libft/ft_strtrim.c \
  lib/libft/ft_substr.c \
  lib/libft/ft_tolower.c \
  lib/libft/ft_toupper.c

# ------------------ get_next_line ------------------
GNL_DIR := lib/get_next_line

GNL_SRCS := \
  $(GNL_DIR)/get_next_line.c \
  $(GNL_DIR)/get_next_line_utils.c

# ------------------ Project SRCS ------------------
# 1) Parse world (used by minishell and tests)
SRCS_LEXER := \
  src/lexer/lexer.c \
  src/lexer/lexer_single_token.c \
  src/lexer/lexer_single_token_word.c \
  src/lexer/lexer_utile.c

SRCS_PARSER := \
  src/parser/parser.c \
  src/parser/parser_basic_act.c \
  src/parser/parser_build_node.c \
  src/parser/parser_redir.c \
  src/parser/parser_redir_next.c \
  src/parser/parser_sub.c \
  src/parser/parser_tk_type.c \
  src/parser/parser_utile.c

SRCS_EXPANDER := \
  src/expander/expander_single_argv.c \
  src/expander/expander_env.c \
  src/expander/expander_utile.c \
  src/expander/expander.c \
  src/expander/expander_var.c

SRCS_SAFE := \
  src/safe_functions/error.c \
  src/safe_functions/quitshell.c \
  src/safe_functions/safe_alloc.c \
  src/safe_functions/safe_list.c \
  src/safe_functions/safe_libft_yp.c \
  src/safe_functions/libft_list.c

# 2) Runtime world (only for minishell binary)
SRCS_RUNTIME := \
  src/core/main.c \
  src/core/shell_cleanup.c \
  src/core/shell_init.c \
  src/core/shell_loop.c \
  src/signals/set_signal_handlers_to_mode.c \
  src/utils/error_exe.c \
  src/utils/ft_list_ops.c \
  src/utils/safe_exe.c \
  src/utils/utils_general.c \
  src/env/env_apis.c \
  src/env/env_init.c\
  src/env/env_utils.c\
  src/builtins/builtin_cmds.c \
  src/builtins/builtin_utils.c \
  src/execution/collect_heredoc.c \
  src/execution/exec_command.c \
  src/execution/exec_pipeline.c \
  src/execution/exec_redirection.c \
  src/execution/heredoc_expander.c\
  src/execution/heredoc_expander_utile.c\
  src/execution/executor.c

SRCS := $(SRCS_RUNTIME) $(SRCS_LEXER) $(SRCS_PARSER) $(SRCS_SAFE) $(GNL_SRCS)

WITH_EXPANDER ?= 1

ifeq ($(WITH_EXPANDER),1)
  SRCS += $(SRCS_EXPANDER)
else
  SRCS += src/expander/expander_stub.c
endif

# Never compile tests/backup files into production binary
SRCS := $(filter-out %_test.c %_tests.c %_backup.c %_old.c,$(SRCS))

#bonus

SRCS_BONUS_REMOVE := \
  src/execution/executor.c \
  src/execution/exec_logical.c \
  src/execution/exec_subshell.c \
  src/parser/parser.c \
  src/parser/parser_sub.c \
  src/parser/parser_redir.c \
  src/parser/parser_build_node.c \
  src/parser/parser_tk_type.c \
  src/lexer/lexer_single_token.c \
  src/lexer/lexer_utile.c

SRCS_BONUS_ADD := \
  bonus/executor_bonus.c \
  bonus/exec_logical.c \
  bonus/exec_subshell.c \
  bonus/parser_bonus.c \
  bonus/parser_sub_bonus.c \
  bonus/parser_redir_bonus.c \
  bonus/parser_build_node_bonus.c \
  bonus/parser_tk_type_bonus.c \
  bonus/lexer_single_token_bonus.c \
  bonus/lexer_utile_bonus.c

SRCS_BONUS := $(filter-out $(SRCS_BONUS_REMOVE),$(SRCS))
SRCS_BONUS += $(SRCS_BONUS_ADD)
OBJS_BONUS := $(SRCS_BONUS:%.c=$(OBJ_DIR)/%.o)

# ------------------ Objects / deps ------------------
OBJS := $(SRCS:%.c=$(OBJ_DIR)/%.o)
DEPS := $(SRCS:%.c=$(DEP_DIR)/%.d)

FTPRINTF_OBJS := $(FTPRINTF_SRCS:%.c=$(OBJ_DIR)/%.o)
FTPRINTF_DEPS := $(FTPRINTF_SRCS:%.c=$(DEP_DIR)/%.d)

# Helper macro to make test objs from a list of sources
define make_objs
$(1:%.c=$(OBJ_DIR)/%.o)
endef

# ------------------ Targets ------------------
all: $(NAME)

$(NAME): $(FTPRINTF_A) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(FTPRINTF_A) $(LDFLAGS) $(LDLIBS) -o $@

$(FTPRINTF_A): $(FTPRINTF_OBJS)
	@mkdir -p $(dir $@)
	ar rcs $@ $^

# Compile rule with deps
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@mkdir -p $(dir $(DEP_DIR)/$*.d)
	$(CC) $(CFLAGS) $(CPPFLAGS) -MMD -MP -MF $(DEP_DIR)/$*.d -c $< -o $@

val: $(NAME)
	@if ! [ -f "ignore.supp" ]; then make ignore; fi
	@valgrind --suppressions=$$(pwd)/ignore.supp --leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes --track-fds=yes -s ./$(NAME)
  
clean:
	rm -rf $(BUILD_DIR)


fclean: clean
	rm -f $(NAME)

re: fclean all

bonus: fclean $(FTPRINTF_A) $(OBJS_BONUS)
	$(CC) $(CFLAGS) $(OBJS_BONUS) $(FTPRINTF_A) $(LDFLAGS) $(LDLIBS) -o $(NAME)

.PHONY: all clean fclean re test bonus

-include $(DEPS)