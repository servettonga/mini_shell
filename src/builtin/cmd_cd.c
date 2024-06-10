/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sehosaf <sehosaf@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 20:37:41 by sehosaf           #+#    #+#             */
/*   Updated: 2024/06/10 21:31:15 by sehosaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_home_keyword(const char *arg);
static int	return_err(char *msg);

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
			return (return_err("minishell: cd: HOME not set"));
	}
	else
		change_to = args[1];
	if (chdir(change_to) != 0)
		return (ft_putstr_fd("minishell: cd: can't cd to ", 2),
			return_err(change_to));
	if (shell->oldpwd == NULL)
		return (return_err("minishell: cd: OLDPWD not set"));
	else
	{
		free(shell->oldpwd);
		shell->oldpwd = ft_strdup(shell->pwd);
	}
	shell->pwd = malloc(PATH_MAX);
	if (getcwd(shell->pwd, PATH_MAX) == NULL)
		return (return_err("minishell: cd: getcwd failed"));
	return (EXIT_SUCCESS);
}

static int	return_err(char *msg)
{
	return (ft_putendl_fd(msg, 2), EXIT_FAILURE);
}

static bool	is_home_keyword(const char *arg)
{
	if (arg[0] == '~' && (arg[1] == '\0'))
		return (true);
	else if (ft_strncmp(arg, "--", 2) == 0)
		return (true);
	else if (ft_strncmp(arg, "$HOME", 6) == 0)
		return (true);
	else if (ft_strncmp(arg, "${HOME}", 8) == 0)
		return (true);
	return (false);
}
