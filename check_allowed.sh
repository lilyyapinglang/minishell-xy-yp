#!/bin/bash

# -----------------------------------
# 42 minishell allowed external functions
# -----------------------------------
allowed_funcs=(
readline rl_clear_history rl_on_new_line rl_replace_line rl_redisplay add_history
printf malloc free write access open read
close fork wait waitpid wait3 wait4 signal
sigaction sigemptyset sigaddset kill exit
getcwd chdir stat lstat fstat unlink execve
dup dup2 pipe opendir readdir closedir
strerror perror isatty ttyname ttyslot ioctl getenv tcsetattr tcgetattr tgetent tgetflag
tgetnum tgetstr tgoto tputs
)

# -----------------------------------
# 42 minishell allowed headers
# -----------------------------------
# 42 minishell allowed headers (quoted to avoid Bash syntax errors)
allowed_headers=(
"<stdio.h>" "<stdlib.h>" "<unistd.h>" "<sys/stat.h>" "<sys/types.h>" "<fcntl.h>" "<signal.h>"
"<dirent.h>" "<errno.h>" "<readline/readline.h>" "<readline/history.h>" "<termios.h>" "<curses.h>" "<term.h>"
)

# -----------------------------------
# Add your Libft functions dynamically from libft.h
# -----------------------------------
if [ -f "libft.h" ]; then
    libft_funcs=$(grep -oE '\bft_[a-zA-Z0-9_]+(?=\s*\()' libft.h | sort -u)
    allowed_funcs+=($libft_funcs)
fi

# -----------------------------------
# Convert arrays to regex patterns
# -----------------------------------
allowed_funcs_pattern=$(printf "|%s" "${allowed_funcs[@]}")
allowed_funcs_pattern=${allowed_funcs_pattern:1}

allowed_headers_pattern=$(printf "|%s" "${allowed_headers[@]}")
allowed_headers_pattern=${allowed_headers_pattern:1}

echo "Scanning source files for function usage and headers..."

# -----------------------------------
# Extract all function calls
# -----------------------------------
used_funcs=$(grep -RhoE '\b[a-zA-Z_][a-zA-Z0-9_]*\s*\(' *.c *.h 2>/dev/null | sed 's/(\s*//' | sort -u)

# Check for disallowed functions
echo "Checking for disallowed functions..."
disallowed_found=0
for func in $used_funcs; do
    if ! [[ $func =~ ^($allowed_funcs_pattern)$ ]]; then
        echo "⚠️ Disallowed function found: $func"
        disallowed_found=1
    fi
done
if [ $disallowed_found -eq 0 ]; then
    echo "✅ All functions are allowed."
fi

# -----------------------------------
# Extract included headers
# -----------------------------------
used_headers=$(grep -RhoE '#include\s*<[^>]+>' *.c *.h 2>/dev/null | sort -u)

# Check for disallowed headers
echo "Checking for disallowed headers..."
header_disallowed=0
for hdr in $used_headers; do
    if ! [[ $hdr =~ ^($allowed_headers_pattern)$ ]]; then
        echo "⚠️ Disallowed header found: $hdr"
        header_disallowed=1
    fi
done
if [ $header_disallowed -eq 0 ]; then
    echo "✅ All headers are allowed."
fi

# -----------------------------------
# Final summary
# -----------------------------------
if [ $disallowed_found -eq 0 ] && [ $header_disallowed -eq 0 ]; then
    echo "🎉 Your code only uses allowed functions and headers."
else
    echo "❌ Please fix the disallowed functions or headers listed above."
fi