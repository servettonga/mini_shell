/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sehosaf <sehosaf@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 19:04:21 by sehosaf           #+#    #+#             */
/*   Updated: 2024/06/09 20:24:57 by sehosaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief The `exit` command exits the shell.
 * @param shell The shell structure.
 */
void	cmd_exit(t_shell *shell, char **args)
{
	if (args[1] == NULL)
		shell->exit_status = EXIT_SUCCESS;
	else if (args[2] == NULL)
	{
		if (ft_isnumber(args[1]))
			shell->exit_status = ft_atoi(args[1]);
		else
		{
			ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
			ft_putstr_fd(args[1], STDERR_FILENO);
			ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
			shell->exit_status = 255;
		}
	}
	else
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		shell->exit_status = EXIT_FAILURE;
	}
	if (shell->exit_status == EXIT_SUCCESS)
		ft_putendl_fd("exit", STDERR_FILENO);
	free_shell_and_exit(shell);
}
