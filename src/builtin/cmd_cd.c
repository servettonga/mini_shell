/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sehosaf <sehosaf@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 20:37:41 by sehosaf           #+#    #+#             */
/*   Updated: 2024/06/12 11:10:55 by sehosaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_home_keyword(const char *arg);

/**
 * @brief Change current working directory
 * @param shell pointer to shell structure
 * @param args arguments passed to cd command including `cd` command itself
 * @return int `EXIT_SUCCESS` if success, `EXIT_FAILURE` otherwise
 */
int	cmd_cd(t_shell *shell, char **args)
{
	char	*change_to;

	if (args[1] == NULL || is_home_keyword(args[1]))
	{
		change_to = get_env_val(shell->env, "HOME");
		if (change_to == NULL)
		{
			ft_putendl_fd("minishell: cd: HOME not set", 2);
			return (EXIT_FAILURE);
		}
	}
	else
		change_to = args[1];
	if (chdir(change_to) != 0)
	{
		ft_putstr_fd("minishell: cd: can't cd to ", 2);
		ft_putendl_fd(change_to, 2);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static bool	is_home_keyword(const char *arg)
{
	if (arg[0] == '~' && (arg[1] == '\0'))
		return (true);
	else if (ft_strcmp(arg, "--") == 0)
		return (true);
	else if (ft_strcmp(arg, "$HOME") == 0)
		return (true);
	else if (ft_strcmp(arg, "${HOME}") == 0)
		return (true);
	return (false);
}
