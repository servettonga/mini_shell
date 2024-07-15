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

static int	absolute_path(t_shell *shell, t_pipeline *p, t_pipeline *cur);
static int	relative_path(t_shell *shell, t_pipeline *p, t_pipeline *cur);

/**
 * @brief Executes the commands in the pipeline and returns the exit status
 * @param pipeline The pipeline to execute
 * @param shell The shell structure
 * @return EXIT_SUCCESS or EXIT_FAILURE
 * @note This function is the entry point for executing a pipeline
 */
int	execute(t_pipeline *pipeline, t_shell *shell)
{
	t_pipeline	*cur;
	int			ret;

	cur = pipeline;
	ret = EXIT_SUCCESS;
	if (create_pipes(pipeline) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	while (cur != NULL)
	{
		if (should_execute(cur->cmd.connection_type, shell->exit_status))
		{
			if (is_builtin(&(cur->cmd)) && cur->cmd.connection_type == CON_NONE)
				ret = execute_builtin(&(cur->cmd), shell);
			else if (is_builtin(&(cur->cmd)))
				ret = handle_process(shell, pipeline, cur);
			else if (cur->cmd.args[0][0] == '/')
				ret = absolute_path(shell, pipeline, cur);
			else
				ret = relative_path(shell, pipeline, cur);
		}
		cur = cur->next;
	}
	return (ret);
}

static int	absolute_path(t_shell *shell, t_pipeline *p, t_pipeline *cur)
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
	return (handle_process(shell, p, cur));
}

static int	relative_path(t_shell *shell, t_pipeline *p, t_pipeline *cur)
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
	return (handle_process(shell, p, cur));
}
