#include "parser.h"

static int validate_var_expansion(char *arg, char *dollar);
static char *get_var_name(char *dollar);

/*
Only 2 types of variable names are accepted:
- $?
- $<alphabetic characters>
*/
void replace_vars(t_pipeline *node)
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
		var_value = get_varval(var_name); // TODO implement char *get_varval(char *name);
		expanded = malloc(ft_strlen(node->cmd.args[i]) - ft_strlen(var_name) + ft_strlen(var_value));
		ft_memcpy(expanded, node->cmd.args[i], start - node->cmd.args[i]);
		ft_memcpy(expanded + (start - node->cmd.args[i]), var_value, ft_strlen(var_value));
		ft_memcpy(expanded + (start - node->cmd.args[i]) + ft_strlen(var_value), start + strlen(var_name) + 1, ft_strlen(node->cmd.args[i]) - ft_strlen(var_name) - (start - node->cmd.args[i]) - 1);
		expanded[ft_strlen(node->cmd.args[i]) - ft_strlen(var_name) + ft_strlen(var_value)] = 0;
		free(node->cmd.args[i]);
		free(var_name);
		node->cmd.args[i] = expanded;
		start = NULL;
	}
}

/*
Validates, whether variable should be replaced by it's value.
Logic - count number of `'` character before `$` sign;
	if even - valid, invalid otherwise
*/
static int validate_var_expansion(char *arg, char *dollar) // FIX - check for " quotes as well
{
	int i;

	i = 0;
	arg = ft_strchr(arg, '\'');
	while (arg && arg < dollar)
	{
		i++;
		arg = ft_strchr(arg + 1, '\'');
	}
	if (i % 2 == 0)
		return (1);
	return (0);
}

/*
Returns variable name. Result is dynamocly allocated and should be freed.
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
