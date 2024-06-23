/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sehosaf <sehosaf@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 21:04:06 by sehosaf           #+#    #+#             */
/*   Updated: 2024/06/23 21:31:05 by sehosaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

static int	handle_absolute_path(t_command *cmd, t_shell *shell);
static int	handle_relative_path(t_command *cmd, t_shell *shell);

int	execute(t_pipeline *p, t_shell *shell)
{
	t_pipeline	*current;

	current = p;
	while (current != NULL)
	{
		if (is_builtin(&(current->cmd)))
			execute_builtin(&(current->cmd), shell);
		else
		{
			if (current->cmd.args[0][0] == '/')
				handle_absolute_path(&(current->cmd), shell);
			else
				handle_relative_path(&(current->cmd), shell);
		}
		current = current->next;
	}
	return (EXIT_SUCCESS);
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
	pid = handle_child_process(cmd, pipe_fd, shell->env);
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
	pid = handle_child_process(cmd, pipe_fd, shell->env);
	handle_parent_process(pid, shell);
	return (EXIT_SUCCESS);
}

/*
int main(int argc, char *argv[])
{
	if (argc > 1)
	{
		t_pipeline *p = malloc(sizeof(t_pipeline));
		if (p == NULL) {
			ft_putendl_fd("Failed to allocate memory for t_pipeline", 2);
			return (EXIT_FAILURE);
		}
		p->cmd.args = malloc(sizeof(char *) * 3);
		if (p->cmd.args == NULL) {
			free(p);
			ft_putendl_fd("Failed to allocate memory for t_pipeline->cmd.args", 2);
			return (EXIT_FAILURE);
		}
		// For instance; ./a.out /bin/ls -lh or ./a.out ls -lh
		p->cmd.args[0] = ft_strdup(argv[1]);
		if (argv[2] != NULL)
			p->cmd.args[1] = ft_strdup(argv[2]);
		p->cmd.args[2] = NULL;
		t_shell *shell = malloc(sizeof(t_shell));
		if (shell == NULL) {
			free_split(p->cmd.args);
			free(p);
			ft_putendl_fd("Failed to allocate memory for t_shell", 2);
			return (EXIT_FAILURE);
		}
		init_environment(shell);
		execute(p, shell);
		free_split(p->cmd.args);
		free(p);
		free_env(shell->env);
		free(shell);
	}
	return (EXIT_SUCCESS);
}
*/
