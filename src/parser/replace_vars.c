#include "parser.h"
#include "environment.h"

static int validate_var_expansion(char *arg, char *dollar);
static char *get_var_name(char *dollar);
static char *get_var_value(const char *name, t_shell *shell);

/*
Only 2 types of variable names are accepted:
- $?
- $<alphabetic characters>
*/
void replace_vars(t_pipeline *node, t_shell *shell)
{
	int i;
	char *start;
	char *var_value;
	char *var_name;
	char *expanded;

	i = 0;
	start = NULL;
	while (node->cmd.args[i])
	{
		if (!start)
			start = ft_strchr(node->cmd.args[i], '$');
		else
			start = ft_strchr(start + 1, '$');
		if (!start)
			i++;
		if (!start || !validate_var_expansion(node->cmd.args[i], start)
			|| !(ft_isalpha(start[1]) || start[1] == '?'))
			continue ;
		var_name = get_var_name(start);
		if (!var_name)
			continue ;
		var_value = get_var_value(var_name, shell);
		expanded = NULL;
		if (var_value)
			expanded = malloc(ft_strlen(node->cmd.args[i]) - ft_strlen(var_name) - 1 + ft_strlen(var_value) + 1);
		if (expanded)
		{
			ft_memcpy(expanded, node->cmd.args[i], start - node->cmd.args[i]);
			ft_memcpy(expanded + (start - node->cmd.args[i]), var_value, ft_strlen(var_value));
			ft_memcpy(expanded + (start - node->cmd.args[i]) + ft_strlen(var_value), start + strlen(var_name) + 1, ft_strlen(node->cmd.args[i]) - ft_strlen(var_name) - (start - node->cmd.args[i]) - 1);
			expanded[ft_strlen(node->cmd.args[i]) - ft_strlen(var_name) + ft_strlen(var_value) - 1] = 0;
			free(node->cmd.args[i]);
			node->cmd.args[i] = expanded;
			start =	NULL;
		}
		free(var_name);
		free(var_value);
	}
}

/*
Validates, whether variable should be replaced by it's value.
If var is in single quotes - invalid.
*/
static int validate_var_expansion(char *arg, char *dollar)
{
	char *dq;
	char *sq;

	while (arg && arg < dollar)
	{
    	dq = ft_strchr(arg, '"');
    	sq = ft_strchr(arg, '\'');
    	if (sq && sq < dollar)
    	{
    	    if (dq && dq < sq)
    	        arg = ft_strchr(dq + 1, '"');
    	    else
    	    {
                arg = ft_strchr(sq + 1, '\'');
                if (arg && arg > dollar)
                    return (0);
    	    }
            if (arg)
                arg++;
    	}
    	else
    	    break ;
	}
	return (1);
}

/*
Returns variable name. Result is dynamicly allocated and should be freed.
Input - pointer on `$` (char before variable name)
*/
static char *get_var_name(char *dollar)
{
	char *res;
	int i;

	if (dollar[1] == '?')
		i = 2;
	else
	{
		i = 1;
		while (ft_isalpha(dollar[i]))
			i++;
	}
	res = malloc(i);
	if (!res)
		return (NULL);
	ft_memcpy(res, dollar + 1, i - 1);
	res[i - 1] = 0;
	return (res);
}

static char *get_var_value(const char *name, t_shell *shell)
{
	char *var_value;

	if (ft_memcmp(name, "?", 2) == 0)
		return ft_itoa(shell->exit_status);
	var_value = get_env_val(shell->env, name);
	return (ft_strdup(var_value));
}
