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
 * @return EXIT_SUCCESS or EXIT_FAILURE
 * @note This function executes the command in the child process
 * and exits the child process
 */
int	handle_process(t_shell *shell, t_pipeline *p, t_pipeline *cur)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
	{
		perror("minishell: Fork failed: ");
		return (EXIT_FAILURE);
	}
	if (pid == 0)
		exit(execute_command(shell, p, cur));
	else
	{
		waitpid(pid, &status, WUNTRACED);
		while (!WIFEXITED(status) && !WIFSIGNALED(status))
			waitpid(pid, &status, WUNTRACED);
		shell->exit_status = WEXITSTATUS(status);
		if (cur->cmd.connection_type != CON_NONE)
			close(p->fd_out);
	}
	return (EXIT_SUCCESS);
}
