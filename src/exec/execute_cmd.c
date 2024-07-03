/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sehosaf <sehosaf@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 21:28:19 by sehosaf           #+#    #+#             */
/*   Updated: 2024/06/24 21:40:23 by sehosaf          ###   ########.fr       */
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
 * @param shell The shell structure
 * @note This function executes the command in a child process
 */
int	execute_command(t_command *cmd, int pipefd[2], t_shell *shell)
{
	char	**env_array;

	handle_input(cmd);
	handle_output(cmd);
	handle_pipe(cmd, pipefd);
	if (is_builtin(cmd))
		return (execute_builtin(cmd, shell));
	env_array = env_list_to_array(shell->env);
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

/**
 * @note If the command has an input file, redirect the input of the command
 * to the input file
 */
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
		if (dup2(in, STDIN_FILENO) < 0)
		{
			perror("minishell: input file error: ");
			close(in);
			exit(EXIT_FAILURE);
		}
		close(in);
	}
}

/**
 * @note If the command has an output file, redirect the output of the command
 * to the output file
 * @note If the output file is in append mode, open the file in append mode
 */
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
		if (dup2(out, STDOUT_FILENO) < 0)
		{
			perror("minishell: output file error: ");
			close(out);
			exit(EXIT_FAILURE);
		}
		close(out);
	}
}

/**
 * @note If the command is a pipe, create a pipe and redirect the output of the
 * current command to the input of the next command
 */
static void	handle_pipe(t_command *command, int pipefd[2])
{
	if (command->connection_type == CON_PIPE)
	{
		if (pipe(pipefd) == -1)
		{
			perror("minishell: pipe error: ");
			exit(EXIT_FAILURE);
		}
		if (dup2(pipefd[1], STDOUT_FILENO) < 0)
		{
			perror("minishell: pipe error: ");
			close(pipefd[0]);
			close(pipefd[1]);
			exit(EXIT_FAILURE);
		}
		close(pipefd[1]);
	}
}
