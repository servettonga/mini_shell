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

// For testing two commands with a pipe and arguments
/*
int main(int argc, char *argv[])
{
	int ret = EXIT_SUCCESS;
	if (argc > 1)
	{
		t_pipeline *p = malloc(sizeof(t_pipeline));
		t_pipeline *p2 = malloc(sizeof(t_pipeline));
		p->cmd.args = malloc(sizeof(char *) * 3);
		p->next = p2;
		p2->cmd.args = malloc(sizeof(char *) * 3);
		// Absolute or relative ./a.out /bin/ls -lh or ./a.out ls -lh
		// ./a.out touch new_file ls -lh
		p->cmd.args[0] = ft_strdup(argv[1]);
		if (argv[2] != NULL)
			p->cmd.args[1] = ft_strdup(argv[2]);
		p->cmd.args[2] = NULL;
		if (argv[3] != NULL)
			p2->cmd.args[0] = ft_strdup(argv[3]);
		if (argv[4] != NULL)
			p2->cmd.args[1] = ft_strdup(argv[4]);
		p2->cmd.args[2] = NULL;
		t_shell *shell = malloc(sizeof(t_shell));
		if (shell == NULL) {
			free_split(p->cmd.args);
			free(p);
			free(p2);
			ft_putendl_fd("Failed to allocate memory", 2);
			return (EXIT_FAILURE);
		}
		init_environment(shell);
		p->cmd.connection_type = CON_PIPE;
		p2->cmd.connection_type = CON_NONE;
		execute(p, shell);
		ret = shell->exit_status;
		free_split(p->cmd.args);
		free_split(p2->cmd.args);
		free(p);
		free(p2);
		free_env(shell->env);
		free(shell);
	}
	return (ret);
}
*/
