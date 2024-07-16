/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sehosaf <sehosaf@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 21:04:06 by sehosaf           #+#    #+#             */
/*   Updated: 2024/06/24 21:46:51 by sehosaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

static int	handle_builtin(t_command *cmd, t_shell *shell);
static int	handle_absolute_path(t_command *cmd, t_shell *shell);
static int	handle_relative_path(t_command *cmd, t_shell *shell);

int	execute(t_pipeline *p, t_shell *shell)
{
	t_pipeline	*current;
	int			ret;

	current = p;
	ret = EXIT_SUCCESS;
	while (current != NULL)
	{
		if (is_builtin(&(current->cmd)))
			ret = handle_builtin(&(current->cmd), shell);
		else
		{
			if (current->cmd.args[0][0] == '/')
				ret = handle_absolute_path(&(current->cmd), shell);
			else
				ret = handle_relative_path(&(current->cmd), shell);
		}
		current = current->next;
	}
	return (ret);
}

static int	handle_builtin(t_command *cmd, t_shell *shell)
{
	pid_t	pid;
	int		pipe_fd[2];

	if (cmd->connection_type == CON_NONE)
		pid = execute_builtin(cmd, shell);
	else
	{
		pid = handle_child_process(cmd, pipe_fd, shell);
		if (pid < 0)
			return (EXIT_FAILURE);
		handle_parent_process(pid, shell);
	}
	return (pid);
}

static int	handle_absolute_path(t_command *cmd, t_shell *shell)
{
	struct stat	st;
	pid_t		pid;
	int			pipe_fd[2];

	if (stat(cmd->args[0], &st) == -1)
	{
		perror("minishell: ");
		shell->exit_status = 127;
		return (EXIT_FAILURE);
	}
	if (S_ISDIR(st.st_mode))
	{
		ft_putendl_fd("minishell: is a directory", 2);
		shell->exit_status = 126;
		return (EXIT_FAILURE);
	}
	pid = handle_child_process(cmd, pipe_fd, shell);
	if (pid < 0)
		return (EXIT_FAILURE);
	handle_parent_process(pid, shell);
	return (EXIT_SUCCESS);
}

static int	handle_relative_path(t_command *cmd, t_shell *shell)
{
	char	*cmd_path;
	pid_t	pid;
	int		pipe_fd[2];

	cmd_path = find_sys_cmd(cmd->args[0], shell->env);
	if (cmd_path == NULL)
	{
		shell->exit_status = 127;
		return (EXIT_FAILURE);
	}
	free(cmd->args[0]);
	cmd->args[0] = cmd_path;
	pid = handle_child_process(cmd, pipe_fd, shell);
	if (pid < 0)
		return (EXIT_FAILURE);
	handle_parent_process(pid, shell);
	return (EXIT_SUCCESS);
}
