
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse.h"
#include "minishellparse.h"
#include "safefunctions.h"

int         lexer(char *input, t_list **token_list, t_shell_context *sh);
int         parser(t_list *token_list, t_ast **ast, t_shell_context *sh);

t_token_type tk_type(t_list *token);
char        *tk_value(t_list *token);
char        *tk_type_to_string(t_token_type type);

/* =========================
 * helpers: token print
 * ========================= */

static const char *token_type_name(t_token_type t)
{
    if (t == TOKEN_WORD)     return "WORD";
    if (t == TOKEN_PIPE)     return "PIPE";
    if (t == TOKEN_AND)      return "AND";
    if (t == TOKEN_OR)       return "OR";
    if (t == TOKEN_LPAREN)   return "LPAREN";
    if (t == TOKEN_RPAREN)   return "RPAREN";
    if (t == REDIR_INPUT)    return "REDIR_IN";
    if (t == REDIR_OUTPUT)   return "REDIR_OUT";
    if (t == REDIR_APPEND)   return "REDIR_APPEND";
    if (t == REDIR_HEREDOC)  return "HEREDOC";
    if (t == TOKEN_EOF)      return "EOF";
    return "UNKNOWN";
}

static void print_tokens(t_list *tok)
{
    int i = 0;

    printf("---- TOKENS ----\n");
    while (tok)
    {
        t_token_type t = tk_type(tok);
        char *v = tk_value(tok);

        printf("[%02d] %-12s", i, token_type_name(t));
        if (v)
            printf(" value=\"%s\"", v);
        else
            printf(" value=NULL");
        printf("  (as_str=%s)\n", tk_type_to_string(t));

        tok = tok->next;
        i++;
    }
    printf("--------------\n");
}

/* =========================
 * helpers: AST print
 * ========================= */

static void print_indent(int depth)
{
    while (depth-- > 0)
        printf("  ");
}

static const char *ast_type_name(t_ast_type t)
{
    if (t == AST_COMMAND)      return "AST_COMMAND";
    if (t == AST_LOGICAL)      return "AST_LOGICAL";
    if (t == AST_PIPELINE)     return "AST_PIPELINE";
    if (t == AST_REDIRECTION)  return "AST_REDIRECTION";
    if (t == AST_SUBSHELL)     return "AST_SUBSHELL";
    return "AST_UNKNOWN";
}

static void print_args(char **args, int depth)
{
    int i = 0;

    print_indent(depth);
    if (!args)
    {
        printf("args=NULL\n");
        return;
    }
    printf("args=[");
    while (args[i])
    {
        printf("\"%s\"", args[i]);
        if (args[i + 1])
            printf(", ");
        i++;
    }
    printf("]\n");
}

static void print_ast(t_ast *node, int depth)
{
    if (!node)
    {
        print_indent(depth);
        printf("(null)\n");
        return;
    }

    print_indent(depth);
    printf("%s\n", ast_type_name(node->type));

    if (node->type == AST_COMMAND)
    {
        print_args(node->u_data.command.args, depth + 1);
    }
    else if (node->type == AST_LOGICAL)
    {
        print_indent(depth + 1);
        printf("op=%s\n", tk_type_to_string(node->u_data.logical.operator));

        print_indent(depth + 1);
        printf("left:\n");
        print_ast(node->u_data.logical.left, depth + 2);

        print_indent(depth + 1);
        printf("right:\n");
        print_ast(node->u_data.logical.right, depth + 2);
    }
    else if (node->type == AST_PIPELINE)
    {
        print_indent(depth + 1);
        printf("left:\n");
        print_ast(node->u_data.pipeline.left, depth + 2);

        print_indent(depth + 1);
        printf("right:\n");
        print_ast(node->u_data.pipeline.right, depth + 2);
    }
    else if (node->type == AST_SUBSHELL)
    {
        print_indent(depth + 1);
        printf("child:\n");
        print_ast(node->u_data.subshell.child, depth + 2);
    }
    else if (node->type == AST_REDIRECTION)
    {
        print_indent(depth + 1);
        printf("redir=%s  file=\"%s\"\n",
            tk_type_to_string(node->u_data.redirection.redir_type),
            node->u_data.redirection.file_path
                ? node->u_data.redirection.file_path
                : "(null)");

        print_indent(depth + 1);
        printf("exe_child:\n");
        print_ast(node->u_data.redirection.exe_child, depth + 2);
    }
}

/* =========================
 * run_test
 * ========================= */

static void reset_parse_error(t_shell_context *sh)
{
    /* 你结构里目前至少有 parsing_error */
    sh->parsing_error = false;
}

static void run_test(const char *input)
{
    t_shell_context sh;
    t_list *tokens = NULL;
    t_ast  *ast = NULL;
    int st;

    memset(&sh, 0, sizeof(sh));
    reset_parse_error(&sh);

    printf("\n==============================\n");
    printf("INPUT: %s\n", input);
    printf("==============================\n");

    st = lexer((char *)input, &tokens, &sh);
    if (st != EXIT_SUCCESS || sh.parsing_error)
    {
        printf("[LEXER] FAILED (st=%d)\n", st);
        return;
    }

    print_tokens(tokens);

    st = parser(tokens, &ast, &sh);
    if (st != EXIT_SUCCESS || sh.parsing_error)
    {
        printf("[PARSER] FAILED (st=%d)\n", st);
        return;
    }

    printf("---- AST ----\n");
    print_ast(ast, 0);
    printf("------------\n");
}

/* =========================
 * main (as you want)
 * ========================= */


int main(int argc, char **argv)
{
    if (argc == 2)
    {
        run_test(argv[1]);
        return (0);
    }

    run_test("ls -l | grep 'file.c' > output");
    run_test("echo \"Hello\" && echo World");
    run_test("echo a||echo b");
    run_test("(echo a && echo b) || echo c");
    run_test("cat << EOF | wc >> out");
    run_test("cmd&&(echo x||echo y)|grep z");
    run_test("single_quotes_unclosed_start'");
    run_test("echo a & echo b");     

    return (0);
}
