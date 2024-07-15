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

static pid_t	absolute_path(t_shell *shell, t_pipeline *p, t_pipeline *cur);
static pid_t	relative_path(t_shell *shell, t_pipeline *p, t_pipeline *cur);
static pid_t	handle_builtin(t_shell *shell, t_pipeline *p, t_pipeline *cur);

/**
 * @brief Executes the commands in the pipeline and returns the exit status
 * @param pipeline The pipeline to execute
 * @param shell The shell structure
 * @return EXIT_SUCCESS or EXIT_FAILURE
 * @note This function is the entry point for executing a pipeline
 */
void	execute(t_pipeline *pipeline, t_shell *shell)
{
	t_pipeline	*cur;
	int			i;
	int			cmds[MAX_CMD];

	cur = pipeline;
	i = 0;
	if (create_pipes(pipeline) == EXIT_FAILURE)
		return ;
	while (cur != NULL)
	{
		if (should_execute(cur->cmd.connection_type, shell->exit_status))
		{
			if (is_builtin(cur->cmd))
				cmds[i] = handle_builtin(shell, pipeline, cur);
			else if (cur->cmd.args[0][0] == '/')
				cmds[i] = absolute_path(shell, pipeline, cur);
			else
				cmds[i] = relative_path(shell, pipeline, cur);
		}
		i++;
		cur = cur->next;
	}
	close_pipes(pipeline);
	execute_pipeline(shell, cmds, i);
}

static pid_t	handle_builtin(t_shell *shell, t_pipeline *p, t_pipeline *cur)
{
	if (cur->cmd.connection_type != CON_NONE
		&& (cur->cmd.infile != NULL || cur->cmd.outfile != NULL))
		return (execute_builtin(cur->cmd, shell));
	else
		return (create_child(shell, p, cur));
}

static pid_t	absolute_path(t_shell *shell, t_pipeline *p, t_pipeline *cur)
{
	struct stat	st;

	if (stat(cur->cmd.args[0], &st) == -1)
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
	return (create_child(shell, p, cur));
}

static pid_t	relative_path(t_shell *shell, t_pipeline *p, t_pipeline *cur)
{
	char	*cmd_path;

	cmd_path = find_sys_cmd(cur->cmd.args[0], shell->env);
	if (cmd_path == NULL)
	{
		shell->exit_status = 127;
		return (EXIT_FAILURE);
	}
	free(cur->cmd.args[0]);
	cur->cmd.args[0] = cmd_path;
	return (create_child(shell, p, cur));
}
