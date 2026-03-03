# Authors
This project has been created as part of the 42 curriculum by ylang and xuewang

# Instruction

## Execution :
Execute in linux with:

```
make && ./minishell
```

To make with bonus:
```
make bonus && ./minishell
```

To clean object files:

```
make clean
```

To remove all generated files:

```
make fclean
```

# Discription

Minishell (a 42 School project) is a simplified version of shell written in C.

The objective of this project is to reproduce core Bash behavior including command execution, pipes, logical operators, redirections, subshells, environment variable expansion signal handling and so on.

## Features

funstion supported:

### Builtins commands suppported
- echo
- cd
- pwd
- export
- unset
- env
- exit

### Operators Supported
- Pipes: `|`
- Redirections:
  - `>` redirects output
  - `>>` redirects output in append mode
  - `<` redirects input
  - `<< DELIMITER` heredoc
- Subshells: `( command )`

### Expansions
- `$VAR`
- `$?`exit status of the most recently executed foreground pipeline
- Heredoc variable expansion (when delimiter is unquoted)

### Signals
- `Ctrl-C`
- `Ctrl-D`
- `Ctrl-\`

### BONUS
- This minishell support `&&` and `||`, while does not support wildcards.


## Structure

In this minishell, the implementation is based on an AST (Abstract Syntax Tree) execution model,
allowing structured parsing and recursive execution similar to bash shell behavior.

The shell follows a multi-stage execution pipeline:

```
INPUT
  ↓
LEXER
  ↓
PARSER
  ↓
AST
  ↓
EXECUTOR (recursive with a loop)
```

### Project Structure

```
bonus/
inc/
lib/
src/
    builtins/
    core/
    env/
    execution/
    expander/
    lexer/
    parser/
    executor/
    safe_functions/
    signals/
    utils/

```

### Lexer
Tokenizes the input string into meaningful tokens:
- WORD
- PIPE
- AND
- OR
- REDIRECTION
- PARENTHESIS
- etc.

### Parser
Builds an Abstract Syntax Tree (AST) based on operator precedence.

Node types include:
- COMMAND
- PIPELINE
- LOGICAL
- REDIRECTION
- SUBSHELL
- SYNTAX_ERROR

### Execution Model

Execution is recursive by a loop and follows shell semantics:

- Logical nodes:
  - Evaluate left side
  - Short-circuit if needed for && and ||
- Pipeline:
  - Create multiple forks
  - Connect via pipe
- Redirections:
  - Applied before command execution
- Subshell:
  - Executed in a separate process context

This structure allows clean separation between parsing and execution.

## Resources

https://taeluf.com/blog/devlog/Designing-A-Lexer/
https://dev.to/balapriya/abstract-syntax-tree-ast-explained-in-plain-english-1h38
https://itnext.io/introduction-to-abstract-syntax-trees-ast-86e2fa455e2c
https://docs.lucee.org/recipes/ast.html
https://github.com/TheoSignore/fdmonitor
https://www.cs.purdue.edu/homes/grr/SystemsProgrammingBook/Book/Chapter5-WritingYourOwnShell.pdf
https://rg1-teaching.mpi-inf.mpg.de/unixffb-ss98/quoting-guide.html
https://brennan.io/2015/01/16/write-a-shell-in-c/


## Examples

```bash
minishell$ echo hello
hello

minishell$ ls | grep .c

minishell$ echo test > file.txt

minishell$ cat file.txt && echo success

minishell$ (echo inside subshell) | cat
```

