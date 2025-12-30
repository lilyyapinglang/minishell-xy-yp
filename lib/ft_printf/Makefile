LIBFT_DIR = ./libft

SOURCE_DIR = ./

NAME = libftprintf.a

LIBFT_SRCS = \
ft_atoi.c \
ft_bzero.c \
ft_calloc.c \
ft_isalnum.c \
ft_isalpha.c \
ft_isascii.c \
ft_isdigit.c \
ft_isprint.c \
ft_itoa.c \
ft_memchr.c \
ft_memcmp.c \
ft_memcpy.c \
ft_memmove.c \
ft_memset.c \
ft_putchar_fd.c \
ft_putendl_fd.c \
ft_putnbr_fd.c \
ft_putstr_fd.c \
ft_split.c \
ft_strchr.c \
ft_strdup.c \
ft_striteri.c \
ft_strjoin.c \
ft_strlcat.c \
ft_strlcpy.c \
ft_strlen.c \
ft_strmapi.c \
ft_strncmp.c \
ft_strnstr.c \
ft_strrchr.c \
ft_strtrim.c \
ft_substr.c \
ft_tolower.c \
ft_toupper.c

PRINTF_SRCS = \
ft_printf.c\
ft_printf_helper.c\
ft_utoa_base.c

# Add correct path to each source 

LIBFT_SRCS_FULL = $(addprefix $(LIBFT_DIR)/, $(LIBFT_SRCS))
PRINTF_SRCS_FULL = $(addprefix $(SOURCE_DIR), $(PRINTF_SRCS))

# Generate object file names 
LIBFT_OBJS = $(LIBFT_SRCS_FULL:.c=.o)
PRINTF_OBJS = $(PRINTF_SRCS_FULL:.c=.o)

# all object file names

OBJS = $(LIBFT_OBJS) $(PRINTF_OBJS)

CC = cc

FLAGS = -Wall -Wextra -Werror -I $(LIBFT_DIR)

AC = ar rcs

RM = rm -f

# Rules
all : $(NAME)

$(NAME): $(OBJS)
	$(AC) $@ $^

%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(FLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all 

.PHONY: all clean fclean re