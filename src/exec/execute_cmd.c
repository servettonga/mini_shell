/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sehosaf <sehosaf@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 21:28:19 by sehosaf           #+#    #+#             */
/*   Updated: 2024/06/23 21:31:40 by sehosaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

static void	handle_input(t_command *command);
static void	handle_output(t_command *command);
static void	handle_pipe(t_command *command, int pipefd[2]);

/**
 * @brief Execute a command
 * @param cmd The command to execute
 * @param pipefd The pipe file descriptors
 * @param env The environment variables
 * @note This function executes the command in a child process
 */
int	execute_command(t_command *cmd, int pipefd[2], t_env *env)
{
	char	**env_array;

	handle_input(cmd);
	handle_output(cmd);
	handle_pipe(cmd, pipefd);
	env_array = env_list_to_array(env);
	if (env_array == NULL)
	{
		ft_putendl_fd("minishell: environment list is empty", 2);
		return (EXIT_FAILURE);
	}
	if (execve(cmd->args[0], cmd->args, env_array) == -1)
	{
		free_split(env_array);
		return (perror("minishell: "), EXIT_FAILURE);
	}
	free_split(env_array);
	return (EXIT_SUCCESS);
}

static void	handle_input(t_command *command)
{
	int	in;

	if (command->infile != NULL)
	{
		in = open(command->infile, O_RDONLY);
		if (in < 0)
		{
			perror("minishell: input file error: ");
			exit(EXIT_FAILURE);
		}
		dup2(in, STDIN_FILENO);
		close(in);
	}
}

static void	handle_output(t_command *command)
{
	int	out;

	if (command->outfile != NULL)
	{
		if (command->outfile_append_mode)
			out = open(command->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			out = open(command->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (out < 0)
		{
			perror("minishell: output file error: ");
			exit(EXIT_FAILURE);
		}
		dup2(out, STDOUT_FILENO);
		close(out);
	}
}

static void	handle_pipe(t_command *command, int pipefd[2])
{
	if (command->connection_type == CON_PIPE)
	{
		pipe(pipefd);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
	}
}
