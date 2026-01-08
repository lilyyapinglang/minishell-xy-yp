
#include "minishellparse.h"
#include "parse.h"
#include "safefunctions.h"

char	*strdup_s(char const *str1, t_tracking_scope scope,
		t_shell_context *sh_ctx)
{
	char	*new_str;

	new_str = ft_strdup(str1);
	s_alloc(new_str, scope, sh_ctx);
	return (new_str);
}
