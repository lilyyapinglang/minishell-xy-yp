#ifndef ENV_H
#define ENV_H

#include "shell_context.h"
#include "list.h"

// typedef struct s_env_var
// {
// 	char				*name;
// 	char				*value;
// 	bool				exported;
// }						t_env_var;

# define DEFAULT_PATH \
	"/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin\
:/usr/local/sbin:/opt/bin:/opt/sbin"

t_list					*init_env(char **envp, t_shell_context *sh_ctx);
void					print_env(bool export_format, t_shell_context *sh_ctx);
char					**build_envp_from_env_list(t_shell_context *sh_ctx);
void					add_new_env_var(t_list **env_list, const char *name,
							const char *value, bool exported,
							t_shell_context *sh_ctx);

t_list					*env_node_find(t_list *env, const char *name);
char					*env_get_value(t_list *env, const char *name);
int						env_set_value(t_shell_context *sh_ctx, const char *name,
							const char *value, bool exported);
int						env_append_value(t_shell_context *sh_ctx,
							const char *name, const char *append_str,
							bool exported);
int						env_unset(t_shell_context *sh_ctx, const char *name);
int						env_mark_exported(t_shell_context *ctx,
							const char *name);
t_env_var				*env_var_from_node(t_list *node);
void					free_env_var(void *content);



#endif