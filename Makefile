# **************************************************************************** #
#                                   minishell                                  #
# **************************************************************************** #

NAME        := minishell

CC          := cc
CFLAGS      := -Wall -Wextra -Werror
CPPFLAGS    := -Iinc -Ilib/ft_printf -Ilib/ft_printf/libft
LDFLAGS     :=
LDLIBS      :=

# -----------------------------------------------------------------------------
# Readline (macOS/Homebrew-friendly)
# - If brew readline exists, use its include/lib dirs
# - Otherwise rely on system defaults
# -----------------------------------------------------------------------------
READLINE_PREFIX := $(shell brew --prefix readline 2>/dev/null)

ifeq ($(READLINE_PREFIX),)
  # fallback: try common locations (won't hurt if they don't exist)
  READLINE_INC :=
  READLINE_LIB :=
else
  READLINE_INC := -I$(READLINE_PREFIX)/include
  READLINE_LIB := -L$(READLINE_PREFIX)/lib
endif

CPPFLAGS    += $(READLINE_INC)
LDFLAGS     += $(READLINE_LIB)
LDLIBS      += -lreadline -lhistory

# -----------------------------------------------------------------------------
# Build directories
# -----------------------------------------------------------------------------
BUILD_DIR   := build
OBJ_DIR     := $(BUILD_DIR)/obj
DEP_DIR     := $(BUILD_DIR)/dep

# -----------------------------------------------------------------------------
# Sources (production)
# IMPORTANT: DO NOT include lexer_test.c / parser_test.c here
# -----------------------------------------------------------------------------
SRCS := \
  src/core/main.c \
  src/core/shell_cleanup.c \
  src/core/shell_init.c \
  src/core/shell_loop.c \
  src/signals/set_signal_handlers_to_mode.c \
  src/utils/error_exe.c \
  src/utils/ft_list_ops.c \
  src/utils/safe_exe.c \
  src/utils/utils_general.c \
  src/safe_functions/error.c \
  src/safe_functions/libft_list.c \
  src/safe_functions/quitshell.c \
  src/safe_functions/safe_alloc.c \
  src/safe_functions/safe_list.c \
  src/safe_functions/secure_libft.c \
  src/env/env.c \
  src/builtins/builtin_cmds.c \
  src/builtins/builtin_utils.c \
  src/lexer/lexer.c \
  src/lexer/lexer_single_token.c \
  src/lexer/lexer_utile.c \
  src/parser/parser.c \
  src/parser/parser_basic_act.c \
  src/parser/parser_build_node.c \
  src/parser/parser_redir.c \
  src/parser/parser_redir_next.c \
  src/parser/parser_sub.c \
  src/parser/parser_tk_type.c \
  src/parser/parser_utile.c \
  src/expander/expand_one_arg.c \
  src/expander/expand_var.c \
  src/expander/expand_var_env.c \
  src/expander/expander.c \
  src/expander/expander_utils.c \
  src/execution/collect_heredoc.c \
  src/execution/exec_command.c \
  src/execution/exec_command_backup.c \
  src/execution/exec_logical.c \
  src/execution/exec_pipeline.c \
  src/execution/exec_redirection.c \
  src/execution/exec_subshell.c \
  src/execution/execution_old.c \
  src/execution/executor.c

# -----------------------------------------------------------------------------
# ft_printf static library
# -----------------------------------------------------------------------------
FTPRINTF_DIR := lib/ft_printf
FTPRINTF_A   := $(BUILD_DIR)/libftprintf.a

FTPRINTF_SRCS := \
  lib/ft_printf/ft_printf.c \
  lib/ft_printf/ft_printf_helper.c \
  lib/ft_printf/ft_utoa_base.c \
  lib/ft_printf/libft/ft_atoi.c \
  lib/ft_printf/libft/ft_bzero.c \
  lib/ft_printf/libft/ft_calloc.c \
  lib/ft_printf/libft/ft_isalnum.c \
  lib/ft_printf/libft/ft_isalpha.c \
  lib/ft_printf/libft/ft_isascii.c \
  lib/ft_printf/libft/ft_isdigit.c \
  lib/ft_printf/libft/ft_isprint.c \
  lib/ft_printf/libft/ft_itoa.c \
  lib/ft_printf/libft/ft_memchr.c \
  lib/ft_printf/libft/ft_memcmp.c \
  lib/ft_printf/libft/ft_memcpy.c \
  lib/ft_printf/libft/ft_memmove.c \
  lib/ft_printf/libft/ft_memset.c \
  lib/ft_printf/libft/ft_putchar_fd.c \
  lib/ft_printf/libft/ft_putendl_fd.c \
  lib/ft_printf/libft/ft_putnbr_fd.c \
  lib/ft_printf/libft/ft_putstr_fd.c \
  lib/ft_printf/libft/ft_split.c \
  lib/ft_printf/libft/ft_strchr.c \
  lib/ft_printf/libft/ft_strdup.c \
  lib/ft_printf/libft/ft_striteri.c \
  lib/ft_printf/libft/ft_strjoin.c \
  lib/ft_printf/libft/ft_strlcat.c \
  lib/ft_printf/libft/ft_strlcpy.c \
  lib/ft_printf/libft/ft_strlen.c \
  lib/ft_printf/libft/ft_strmapi.c \
  lib/ft_printf/libft/ft_strncmp.c \
  lib/ft_printf/libft/ft_strnstr.c \
  lib/ft_printf/libft/ft_strrchr.c \
  lib/ft_printf/libft/ft_strtrim.c \
  lib/ft_printf/libft/ft_substr.c \
  lib/ft_printf/libft/ft_tolower.c \
  lib/ft_printf/libft/ft_toupper.c

# -----------------------------------------------------------------------------
# Objects / deps
# -----------------------------------------------------------------------------
OBJS := $(SRCS:%.c=$(OBJ_DIR)/%.o)
DEPS := $(SRCS:%.c=$(DEP_DIR)/%.d)

FTPRINTF_OBJS := $(FTPRINTF_SRCS:%.c=$(OBJ_DIR)/%.o)
FTPRINTF_DEPS := $(FTPRINTF_SRCS:%.c=$(DEP_DIR)/%.d)

# -----------------------------------------------------------------------------
# Test targets: build separate binaries without polluting minishell
# -----------------------------------------------------------------------------
TEST_DIR       := tests
TEST_BUILD_DIR := $(BUILD_DIR)/tests

# Use existing test sources from src/ (you can later move them into tests/)
LEXER_TEST_SRC := src/lexer/lexer_test.c
PARSER_TEST_SRC:= src/parser/parser_test.c

LEXER_TEST_BIN := $(TEST_BUILD_DIR)/lexer_test
PARSER_TEST_BIN:= $(TEST_BUILD_DIR)/parser_test

LEXER_TEST_OBJ := $(OBJ_DIR)/$(LEXER_TEST_SRC:%.c=%.o)
PARSER_TEST_OBJ:= $(OBJ_DIR)/$(PARSER_TEST_SRC:%.c=%.o)

# Minimal link sets for tests:
# - lexer_test needs lexer + safe/utils + ft_printf (often)
# - parser_test needs lexer + parser + safe/utils (+ maybe expander depending on test)
LEXER_TEST_LINK_OBJS := \
  $(OBJ_DIR)/src/lexer/lexer.o \
  $(OBJ_DIR)/src/lexer/lexer_single_token.o \
  $(OBJ_DIR)/src/lexer/lexer_utile.o \
  $(OBJ_DIR)/src/safe_functions/error.o \
  $(OBJ_DIR)/src/safe_functions/libft_list.o \
  $(OBJ_DIR)/src/safe_functions/quitshell.o \
  $(OBJ_DIR)/src/safe_functions/safe_alloc.o \
  $(OBJ_DIR)/src/safe_functions/safe_list.o \
  $(OBJ_DIR)/src/safe_functions/secure_libft.o \
  $(OBJ_DIR)/src/utils/ft_list_ops.o \
  $(OBJ_DIR)/src/utils/utils_general.o \
  $(OBJ_DIR)/src/utils/error_exe.o \
  $(OBJ_DIR)/src/utils/safe_exe.o

PARSER_TEST_LINK_OBJS := \
  $(LEXER_TEST_LINK_OBJS) \
  $(OBJ_DIR)/src/parser/parser.o \
  $(OBJ_DIR)/src/parser/parser_basic_act.o \
  $(OBJ_DIR)/src/parser/parser_build_node.o \
  $(OBJ_DIR)/src/parser/parser_redir.o \
  $(OBJ_DIR)/src/parser/parser_redir_next.o \
  $(OBJ_DIR)/src/parser/parser_sub.o \
  $(OBJ_DIR)/src/parser/parser_tk_type.o \
  $(OBJ_DIR)/src/parser/parser_utile.o

# -----------------------------------------------------------------------------
# Default rule
# -----------------------------------------------------------------------------
all: $(NAME)

$(NAME): $(FTPRINTF_A) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(FTPRINTF_A) $(LDFLAGS) $(LDLIBS) -o $@

# -----------------------------------------------------------------------------
# ft_printf build
# -----------------------------------------------------------------------------
$(FTPRINTF_A): $(FTPRINTF_OBJS)
	@mkdir -p $(dir $@)
	ar rcs $@ $^

# -----------------------------------------------------------------------------
# Compile rules with dependency generation
# -----------------------------------------------------------------------------
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@mkdir -p $(dir $(DEP_DIR)/$*.d)
	$(CC) $(CFLAGS) $(CPPFLAGS) -MMD -MP -MF $(DEP_DIR)/$*.d -c $< -o $@

# -----------------------------------------------------------------------------
# Tests
# -----------------------------------------------------------------------------
test: test_lexer test_parser

test_lexer: $(FTPRINTF_A) $(LEXER_TEST_OBJ) $(LEXER_TEST_LINK_OBJS)
	@mkdir -p $(TEST_BUILD_DIR)
	$(CC) $(CFLAGS) $(LEXER_TEST_OBJ) $(LEXER_TEST_LINK_OBJS) $(FTPRINTF_A) \
		$(LDFLAGS) $(LDLIBS) -o $(LEXER_TEST_BIN)
	@echo "Built: $(LEXER_TEST_BIN)"

test_parser: $(FTPRINTF_A) $(PARSER_TEST_OBJ) $(PARSER_TEST_LINK_OBJS)
	@mkdir -p $(TEST_BUILD_DIR)
	$(CC) $(CFLAGS) $(PARSER_TEST_OBJ) $(PARSER_TEST_LINK_OBJS) $(FTPRINTF_A) \
		$(LDFLAGS) $(LDLIBS) -o $(PARSER_TEST_BIN)
	@echo "Built: $(PARSER_TEST_BIN)"

# -----------------------------------------------------------------------------
# Housekeeping
# -----------------------------------------------------------------------------
clean:
	rm -rf $(BUILD_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re test test_lexer test_parser

-include $(DEPS)
-include $(FTPRINTF_DEPS)