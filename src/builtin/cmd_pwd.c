/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_pwd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sehosaf <sehosaf@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 14:08:08 by sehosaf           #+#    #+#             */
/*   Updated: 2024/06/10 21:01:33 by sehosaf          ###   ########.fr       */
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
	if (shell->pwd == NULL || (shell->pwd && shell->pwd[0] == '\0'))
	{
		shell->pwd = (char *)malloc(PATH_MAX);
		if (shell->pwd == NULL)
			return (perror("pwd"), EXIT_FAILURE);
		if (getcwd(shell->pwd, PATH_MAX) == NULL)
			return (perror("pwd"), EXIT_FAILURE);
	}
	ft_putendl_fd(shell->pwd, STDOUT_FILENO);
	return (EXIT_SUCCESS);
}
