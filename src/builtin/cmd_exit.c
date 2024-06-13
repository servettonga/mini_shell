/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sehosaf <sehosaf@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 19:04:21 by sehosaf           #+#    #+#             */
/*   Updated: 2024/06/13 16:55:19 by sehosaf          ###   ########.fr       */
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
	if (args[2] != NULL)
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		return ;
	}
	if (ft_isnumber(args[1]))
		shell->exit_status = ft_atol(args[1]);
	else
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(args[1], STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		shell->exit_status = 255;
	}
	if (shell->exit_status == EXIT_SUCCESS)
		ft_putendl_fd("exit", STDERR_FILENO);
	if (shell->exit_status < 0)
		shell->exit_status = 256 + shell->exit_status;
	if (shell->exit_status > 255)
		shell->exit_status %= 256;
	cleanup_and_exit_shell(shell);
}
