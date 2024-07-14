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
 * @brief Handle the parent process
 * @param pid The process id
 * @param shell The shell structure
 * @note This function waits for the child process to finish
 * and sets the exit status of the shell
 */
void handle_parent_process(pid_t pid, t_shell *shell, t_pipeline *p)
{
	int	status;

	waitpid(pid, &status, WUNTRACED);
	while (!WIFEXITED(status) && !WIFSIGNALED(status))
		waitpid(pid, &status, WUNTRACED);
	shell->exit_status = WEXITSTATUS(status);
	close(p->fd_out);
}

/**
 * @brief Handle the child process
 * @param cmd The command to execute
 * @param shell The shell structure
 * @return The process id of the child process
 * @note This function executes the command in the child process
 * and exits the child process
 * @note Signal handlers are set to default in the child process
 */
pid_t handle_child_process(t_command *cmd, t_shell *shell, t_pipeline *p)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("minishell: Fork failed: ");
		return (pid);
	}
	if (pid == 0)
		exit(execute_command(cmd, shell, p));
	return (pid);
}
