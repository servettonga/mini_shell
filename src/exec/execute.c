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
			if (is_builtin(&(current->cmd)) && current->cmd.connection_type == CON_NONE)
				ret = execute_builtin(&(current->cmd), shell);
			else if (is_builtin(&(current->cmd)))
				ret = handle_process(&(current->cmd), shell, p);
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

static int handle_absolute_path(t_command *cmd, t_shell *shell, t_pipeline *p)
{
	struct stat	st;

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
	return (handle_process(cmd, shell, p));
}

static int handle_relative_path(t_command *cmd, t_shell *shell, t_pipeline *p)
{
	char	*cmd_path;

	cmd_path = find_sys_cmd(cmd->args[0], shell->env);
	if (cmd_path == NULL)
	{
		shell->exit_status = 127;
		return (EXIT_FAILURE);
	}
	free(cmd->args[0]);
	cmd->args[0] = cmd_path;
	return (handle_process(cmd, shell, p));
}
