/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_vars.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sehosaf <sehosaf@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 09:32:14 by sehosaf           #+#    #+#             */
/*   Updated: 2024/07/17 21:46:00 by sehosaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "environment.h"

static int	validate_var_expansion(char *arg, const char *dollar);
static char	*get_start(const t_pipeline *node, int i, char *start);
static void	handle_expanded(t_pipeline *node, int i, char *start, t_env *env);
static void	*alloc_expanded(const t_pipeline *node, int i, const char *value,
				const char *name);
static char *get_var_value(const char *name, t_shell *shell);

/*
	Only 2 types of variable names are accepted:
	- $?
	- $<alphabetic characters>
*/
void	replace_vars(t_pipeline *node, t_shell *shell)
{
	int		i;
	char	*start;

	i = 0;
	while (node->cmd.args[i])
	{
		start = get_start(node, i, start);
		if (!start)
			i++;
		if (!start || !validate_var_expansion(node->cmd.args[i], start)
			|| !(ft_isalpha(start[1]) || start[1] == '?'))
			continue ;
		handle_expanded(node, i, start, env);
		start = NULL;
	}
}

static char	*get_start(const t_pipeline *node, int i, char *start)
{
	if (!start)
		start = ft_strchr(node->cmd.args[i], '$');
	else
		start = ft_strchr(start + 1, '$');
	return (start);
}

static void	*alloc_expanded(const t_pipeline *node, int i, const char *value,
			const char *name)
{
	return (malloc(ft_strlen(node->cmd.args[i])
			- ft_strlen(name) - 1 + ft_strlen(value) + 1));
}

static void	handle_expanded(t_pipeline *node, int i, char *start, t_env *env)
{
	char			*exp;
	char			*value;
	char			*name;
	unsigned long	loc;

	exp = NULL;
	name = get_var_name(start);
	if (!name)
		return ;
	value = get_env_val(env, name);
	if (value)
		exp = alloc_expanded(node, i, value, name);
	if (exp)
	{
		loc = start - node->cmd.args[i];
		ft_memcpy(exp, node->cmd.args[i], start - node->cmd.args[i]);
		ft_memcpy(exp + loc, value, ft_strlen(value));
		ft_memcpy(exp + loc + ft_strlen(value), start + strlen(name)
			+ 1, ft_strlen(node->cmd.args[i]) - ft_strlen(name) - loc - 1);
		exp[ft_strlen(node->cmd.args[i]) - ft_strlen(name)
			+ ft_strlen(value) - 1] = 0;
		free(node->cmd.args[i]);
		node->cmd.args[i] = exp;
	}
	free(name);
}

/*
	Validates, whether variable should be replaced by its value.
	If var is in single quotes - invalid.
*/
static int	validate_var_expansion(char *arg, const char *dollar)
{
	char	*dq;
	char	*sq;

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
