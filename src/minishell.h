#ifndef MINISHELL_H
#define MINISHELL_H


// define a comman table for execttion 

typedef struct s_redir {
    int type; // <, >, >>, << (HEREDOC)
    char *filename; //delimiter for heredoc
} t_redir;

typedef struct s_cmd{
    char **argv;
    t_redir *redirs;
    int pipe_in;
    int pipe_out;
}t_cmd;

typedef struct s_cmd_table{
    t_cmd *cmds;
    int nb_nums;
} t_cmd_table;

#endif