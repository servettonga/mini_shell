/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_pwd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sehosaf <sehosaf@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 14:08:08 by sehosaf           #+#    #+#             */
/*   Updated: 2024/06/04 08:57:50 by sehosaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief The `pwd` command writes the absolute pathname of the current working
 * directory to the standard output.
 * @param shell The shell structure.
 */
int	cmd_pwd(t_shell *shell)
{
	if (shell->cwd == NULL || (shell->cwd && shell->cwd[0] == '\0'))
	{
		shell->cwd = (char *)malloc(PATH_MAX);
		if (shell->cwd == NULL)
			return (perror("pwd"), EXIT_FAILURE);
		if (getcwd(shell->cwd, PATH_MAX) == NULL)
			return (perror("pwd"), EXIT_FAILURE);
	}
	ft_putendl_fd(shell->cwd, STDOUT_FILENO);
	return (EXIT_SUCCESS);
}
