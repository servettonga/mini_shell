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
 * @brief Creates a child process to execute the command
 * @param shell The shell structure
 * @param p The pipeline of the command
 * @param cur The current command in the pipeline to execute
 * @return Process ID of the child process
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

/**
 * @brief Executes the pipeline of commands in async mode
 * @param shell The shell structure
 * @param cmds The array of process IDs
 * @param num_cmds The number of commands in the pipeline
 */
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

/**
 * @brief Executes the pipeline of commands in sync mode
 * @param shell The shell structure
 * @param pid The process ID of the child process
 */
void	execute_pipe(t_shell *shell, pid_t pid)
{
	int	status;

	waitpid(pid, &status, WUNTRACED);
	while (!WIFEXITED(status) && !WIFSIGNALED(status))
		waitpid(pid, &status, WUNTRACED);
	shell->exit_status = WEXITSTATUS(status);
}

/**
 * @brief Checks if the pipeline is asynchronous
 * @param pipeline The pipeline of commands
 * @return true if the pipeline is asynchronous, false otherwise
 */
bool	is_async(t_pipeline *pipeline)
{
	t_pipeline	*current;

	current = pipeline;
	while (current)
	{
		if (current->cmd.connection_type == CON_OR
			|| current->cmd.connection_type == CON_AND)
			return (false);
		current = current->next;
	}
	return (true);
}
