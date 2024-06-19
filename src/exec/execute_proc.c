/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_proc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sehosaf <sehosaf@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 21:29:30 by sehosaf           #+#    #+#             */
/*   Updated: 2024/06/19 21:30:30 by sehosaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

void	handle_parent_process(pid_t pid, t_shell *shell)
{
	int	status;

	waitpid(pid, &status, WUNTRACED);
	while (!WIFEXITED(status) && !WIFSIGNALED(status))
		waitpid(pid, &status, WUNTRACED);
	shell->exit_status = WEXITSTATUS(status);
}

pid_t	handle_child_process(t_command *cmd, int pipefd[2], t_env *env)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		execute_command(cmd, pipefd, env);
		// TODO: Handle the case when the command is not found
		exit(EXIT_SUCCESS);
	}
	return (pid);
}
