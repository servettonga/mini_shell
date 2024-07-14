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

static int handle_builtin(t_command *cmd, t_shell *shell, t_pipeline *p);
static int handle_absolute_path(t_command *cmd, t_shell *shell, t_pipeline *p);
static int handle_relative_path(t_command *cmd, t_shell *shell, t_pipeline *p);

int	execute(t_pipeline *p, t_shell *shell)
{
	t_pipeline	*current;
	int			ret;

	current = p;
	ret = EXIT_SUCCESS;
	if (create_pipes(p) == EXIT_FAILURE)
			return(EXIT_FAILURE);
	while (current != NULL)
	{
		if (should_execute(current->cmd.connection_type, shell->exit_status))
		{
			if (is_builtin(&(current->cmd)))
				ret = handle_builtin(&(current->cmd), shell, current);
			else {
				if (current->cmd.args[0][0] == '/')
					ret = handle_absolute_path(&(current->cmd), shell, current);
				else
					ret = handle_relative_path(&(current->cmd), shell, current);
			}
		}
		current = current->next;
	}
	return (ret);
}

static int handle_builtin(t_command *cmd, t_shell *shell, t_pipeline *p)
{
	pid_t	pid;

	if (cmd->connection_type == CON_NONE)
		pid = execute_builtin(cmd, shell);
	else
	{
		pid = handle_child_process(cmd, shell, p);
		if (pid < 0)
			return (EXIT_FAILURE);
		handle_parent_process(pid, shell, p);
	}
	return (pid);
}

static int handle_absolute_path(t_command *cmd, t_shell *shell, t_pipeline *p)
{
	struct stat	st;
	pid_t		pid;

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
	pid = handle_child_process(cmd, shell, p);
	if (pid < 0)
		return (EXIT_FAILURE);
	handle_parent_process(pid, shell, p);
	return (EXIT_SUCCESS);
}

static int handle_relative_path(t_command *cmd, t_shell *shell, t_pipeline *p)
{
	char	*cmd_path;
	pid_t	pid;

	cmd_path = find_sys_cmd(cmd->args[0], shell->env);
	if (cmd_path == NULL)
	{
		shell->exit_status = 127;
		return (EXIT_FAILURE);
	}
	free(cmd->args[0]);
	cmd->args[0] = cmd_path;
	pid = handle_child_process(cmd, shell, p);
	if (pid < 0)
		return (EXIT_FAILURE);
	handle_parent_process(pid, shell, p);
	return (EXIT_SUCCESS);
}
