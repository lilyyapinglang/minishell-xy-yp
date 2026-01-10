NAME := minishell

CC := cc
CFLAGS := -Wall -Wextra -Werror
CPPFLAGS := -Iinc -Ilib/ft_printf -Ilib/libft
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

# ------------------ Project SRCS ------------------
# 1) Parse world (used by minishell and tests)
SRCS_LEXER := \
  src/lexer/lexer.c \
  src/lexer/lexer_single_token.c \
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
  src/env/env.c \
  src/builtins/builtin_cmds.c \
  src/builtins/builtin_utils.c \
  src/execution/collect_heredoc.c \
  src/execution/exec_command.c \
  src/execution/exec_logical.c \
  src/execution/exec_pipeline.c \
  src/execution/exec_redirection.c \
  src/execution/exec_subshell.c \
  src/execution/executor.c


SRCS := $(SRCS_RUNTIME) $(SRCS_LEXER) $(SRCS_PARSER) $(SRCS_SAFE)

WITH_EXPANDER ?= 1

ifeq ($(WITH_EXPANDER),1)
  SRCS += $(SRCS_EXPANDER)
else
  SRCS += src/expander/expander_stub.c
endif

# Never compile tests/backup files into production binary
SRCS := $(filter-out %_test.c %_tests.c %_backup.c %_old.c,$(SRCS))

# ------------------ Tests (standalone) ------------------
LEXER_TEST_SRC    := tests/unit/lexer_test.c
PARSER_TEST_SRC   := tests/unit/parser_test.c
EXPANDER_TEST_SRC := tests/unit/expander_test.c

# For tests, do NOT link runtime modules by default
LEXER_TEST_SRCS    := $(SRCS_LEXER) $(SRCS_SAFE) $(LEXER_TEST_SRC)
PARSER_TEST_SRCS   := $(SRCS_LEXER) $(SRCS_PARSER) $(SRCS_SAFE) $(PARSER_TEST_SRC)
EXPANDER_TEST_SRCS := $(SRCS_LEXER) $(SRCS_EXPANDER) $(SRCS_SAFE) $(EXPANDER_TEST_SRC)

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

# ------------------ Test targets ------------------
test: test_lexer test_parser test_expander

test_lexer: $(FTPRINTF_A) $(call make_objs,$(LEXER_TEST_SRCS))
	@mkdir -p $(TEST_DIR)
	$(CC) $(CFLAGS) $(call make_objs,$(LEXER_TEST_SRCS)) $(FTPRINTF_A) \
		$(LDFLAGS) $(LDLIBS) -o $(TEST_DIR)/lexer_test
	@echo "Built: $(TEST_DIR)/lexer_test"

test_parser: $(FTPRINTF_A) $(call make_objs,$(PARSER_TEST_SRCS))
	@mkdir -p $(TEST_DIR)
	$(CC) $(CFLAGS) $(call make_objs,$(PARSER_TEST_SRCS)) $(FTPRINTF_A) \
		$(LDFLAGS) $(LDLIBS) -o $(TEST_DIR)/parser_test
	@echo "Built: $(TEST_DIR)/parser_test"

test_expander: $(FTPRINTF_A) $(call make_objs,$(EXPANDER_TEST_SRCS))
	@mkdir -p $(TEST_DIR)
	$(CC) $(CFLAGS) $(call make_objs,$(EXPANDER_TEST_SRCS)) $(FTPRINTF_A) \
		$(LDFLAGS) $(LDLIBS) -o $(TEST_DIR)/expander_test
	@echo "Built: $(TEST_DIR)/expander_test"

run_test_lexer: test_lexer
	./$(TEST_DIR)/lexer_test

run_test_parser: test_parser
	./$(TEST_DIR)/parser_test

run_test_expander: test_expander
	./$(TEST_DIR)/expander_test

run_test: run_test_lexer run_test_parser run_test_expander

val: $(NAME)
	@if ! [ -f "ignore.supp" ]; then make ignore; fi
	@valgrind --suppressions=$$(pwd)/ignore.supp --leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes --track-fds=yes -s ./$(NAME)
  
clean:
	rm -rf $(BUILD_DIR)


fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re test test_lexer test_parser test_expander \
        run_test run_test_lexer run_test_parser run_test_expander
-include $(DEPS)
-include $(FTPRINTF_DEPS)