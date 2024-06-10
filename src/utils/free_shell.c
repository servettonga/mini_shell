/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sehosaf <sehosaf@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 19:16:31 by sehosaf           #+#    #+#             */
/*   Updated: 2024/06/10 21:32:31 by sehosaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// TODO: Add further functions to handle the allocated memory for a clean exit
/**
 * @brief Frees the shell structure and exits the shell with the exit status.
 * @param shell The shell structure to free
 */
void	free_shell_and_exit(t_shell *shell)
{
	int	exit_status;

	exit_status = shell->exit_status;
	if (exit_status < 0 || exit_status > 255)
		exit_status = 255;
	free_env(shell->env);
	free(shell->pwd);
	free(shell);
	exit(exit_status);
}
