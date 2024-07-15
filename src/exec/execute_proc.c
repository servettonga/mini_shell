/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_proc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sehosaf <sehosaf@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 21:29:30 by sehosaf           #+#    #+#             */
/*   Updated: 2024/06/24 21:42:11 by sehosaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

/**
 * @brief Handles the execution process of a command in the pipeline
 * @param shell The shell structure
 * @param p The pipeline of the command
 * @param cur The current command in the pipeline to execute
 * @return Process ID of the child process
 * @note This function executes the command in the child process
 * and exits the child process
 */
pid_t	create_child(t_shell *shell, t_pipeline *p, t_pipeline *cur)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("minishell: Fork failed: ");
		return (EXIT_FAILURE);
	}
	if (pid == 0)
		exit(execute_command(shell, p, cur));
	return (pid);
}

void	execute_pipeline(t_shell *shell, int *cmds, int num_cmds)
{
	int	i;

	i = 0;
	while (i < num_cmds)
	{
		waitpid(cmds[i], &(shell->exit_status), WUNTRACED);
		shell->exit_status = WEXITSTATUS(shell->exit_status);
		i++;
	}
}
