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

static void handle_input(t_command *cmd);
static void handle_output(t_command *cmd);
static void handle_pipe(t_pipeline *p);

/**
 * @brief Execute a command
 * @param cmd The command to execute
 * @param p The pipe file descriptors
 * @param shell The shell structure
 * @note This function executes the command in a child process
 */
int execute_command(t_command *cmd, t_shell *shell, t_pipeline *p)
{
	char	**env_array;

	handle_input(cmd);
	handle_output(cmd);
	handle_pipe(p);
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
static void handle_input(t_command *cmd)
{
	int	in;

	if (cmd->infile != NULL)
	{
		in = open(cmd->infile, O_RDONLY);
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
static void handle_output(t_command *cmd)
{
	int	out;

	if (cmd->outfile != NULL)
	{
		if (cmd->outfile_append_mode)
			out = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			out = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
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

static void handle_pipe(t_pipeline *p)
{
	if (p->prev != NULL)
		dup2(p->prev->fd_in, STDIN_FILENO);
	if (p->next != NULL)
		dup2(p->fd_out, STDOUT_FILENO);
	close_pipes(p, p);
}