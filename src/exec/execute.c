/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sehosaf <sehosaf@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 21:04:06 by sehosaf           #+#    #+#             */
/*   Updated: 2024/06/17 21:08:27 by sehosaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_builtin(t_command *command)
{
	char		*cmd_name;
	int			i;
	const char	*builtin_commands[] = {"cd", "echo", "env", "exit", "export",
		"pwd", "unset", NULL};

	i = 0;
	cmd_name = command->args[0];
	while (builtin_commands[i])
	{
		if (ft_strcmp(cmd_name, builtin_commands[i]) == 0)
			return (true);
		i++;
	}
	return (false);
}

static void	execute_builtin(t_command *command, t_shell *shell)
{
	char	*cmd_name;

	cmd_name = command->args[0];
	if (ft_strcmp(cmd_name, "cd") == 0)
		cmd_cd(shell, command->args);
	else if (ft_strcmp(cmd_name, "echo") == 0)
		cmd_echo((const char **)command->args);
	else if (ft_strcmp(cmd_name, "env") == 0)
		cmd_env(shell);
	else if (ft_strcmp(cmd_name, "exit") == 0)
		cmd_exit(shell, command->args);
	else if (ft_strcmp(cmd_name, "export") == 0)
		cmd_export(shell, command->args[1]);
	else if (ft_strcmp(cmd_name, "pwd") == 0)
		cmd_pwd();
	else if (ft_strcmp(cmd_name, "unset") == 0)
		cmd_unset(shell, command->args[1]);
}

static void	handle_input(t_command *command)
{
	int	in;

	if (command->infile != NULL)
	{
		in = open(command->infile, O_RDONLY);
		if (in < 0)
		{
			perror("minishell");
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
			perror("minishell");
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

char	**env_list_to_array(t_env *env)
{
	int		count;
	char	**env_array;
	t_env	*e;
	int		i;

	e = env;
	count = get_env_size(env);
	env_array = malloc((count + 1) * sizeof(char *));
	if (env_array == NULL)
		return (NULL);
	i = 0;
	while (e != NULL)
	{
		env_array[i] = malloc(strlen(e->key) + strlen(e->value) + 2);
		if (env_array[i] == NULL)
			return (free_split(env_array), NULL);
		ft_strlcat(env_array[i], e->key, ft_strlen(e->key) + 1);
		ft_strlcat(env_array[i], "=", 2);
		ft_strlcat(env_array[i], e->value, ft_strlen(e->value) + 1);
		i++;
		e = e->next;
	}
	env_array[i] = NULL;
	return (env_array);
}

static void	execute_command(t_command *cmd, int pipefd[2], t_env *env)
{
	char	**env_array;

	handle_input(cmd);
	handle_output(cmd);
	handle_pipe(cmd, pipefd);
	env_array = env_list_to_array(env);
	if (env_array == NULL)
		return ;
	if (execve(cmd->args[0], cmd->args, env_array) == -1)
	{
		perror("minishell");
		exit(EXIT_FAILURE);
	}
	free_split(env_array);
}

static void	handle_parent_process(pid_t pid, t_shell *shell)
{
	int	status;

	waitpid(pid, &status, WUNTRACED);
	while (!WIFEXITED(status) && !WIFSIGNALED(status))
		waitpid(pid, &status, WUNTRACED);
	shell->exit_status = WEXITSTATUS(status);
}

static pid_t	handle_child_process(t_command *cmd, int pipefd[2], t_env *env)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		execute_command(cmd, pipefd, env);
		// TODO: Handle the case when the command is not found
		exit(EXIT_SUCCESS);
	}
	return (pid);
}

static char	*find_sys_cmd(char *cmd, t_env *env)
{
	char	*path;
	char	**path_dirs;
	char	*sys_cmd;
	int		i;

	path = get_env_val(env, "PATH");
	if (path == NULL)
		return (NULL);
	path_dirs = ft_split(path, ':');
	if (path_dirs == NULL)
		return (NULL);
	i = -1;
	while (path_dirs[++i] != NULL)
	{
		sys_cmd = ft_strjoin(path_dirs[i], "/");
		if (sys_cmd == NULL)
			return (free_split(path_dirs), NULL);
		sys_cmd = ft_strjoin(sys_cmd, cmd);
		if (sys_cmd == NULL)
			return (free_split(path_dirs), NULL);
		if (access(sys_cmd, F_OK) == 0)
			return (sys_cmd);
		free(sys_cmd);
	}
	return (free_split(path_dirs), NULL);
}

int	execute(t_pipeline *p, t_shell *shell)
{
	int			pipefd[2];
	pid_t		pid;
	t_pipeline	*current;
	char		*cmd_path;
	struct stat	s;

	current = p;
	while (current != NULL)
	{
		if (is_builtin(&(current->cmd)))
			execute_builtin(&(current->cmd), shell);
		else
		{
			if (current->cmd.args[0][0] == '/')
			{
				stat(current->cmd.args[0], &s);
				if (S_ISDIR(s.st_mode))
				{
					printf("%s is a directory\n", current->cmd.args[0]);
					// TODO: Handle the case when the command is a directory
				}
				else
				{
					pid = handle_child_process(&(current->cmd), pipefd, shell->env);
					handle_parent_process(pid, shell);
				}
			}
			else
			{
				printf("%s is a system command\n", current->cmd.args[0]);
				// TODO: Execute the system command
				cmd_path = find_sys_cmd(current->cmd.args[0], shell->env);
				if (cmd_path == NULL)
				{
					printf("minishell: %s: not found\n", current->cmd.args[0]);
					return (EXIT_FAILURE);
				}
				free(current->cmd.args[0]);
				current->cmd.args[0] = cmd_path;
				pid = handle_child_process(&(current->cmd), pipefd, shell->env);
				handle_parent_process(pid, shell);
			}
		}
		current = current->next;
	}
	return (EXIT_SUCCESS);
}

/*
int main(int argc, char *argv[])
{
	(void)argc;
	t_pipeline *p = malloc(sizeof(t_pipeline));
	if (p == NULL) {
		fprintf(stderr, "Failed to allocate memory for t_pipeline\n");
		return EXIT_FAILURE;
	}

	p->cmd.args = malloc(sizeof(char *) * 2);
	if (p->cmd.args == NULL) {
		fprintf(stderr, "Failed to allocate memory for args\n");
		return EXIT_FAILURE;
	}
	// For instance; ./a.out /bin/ls -lh or ./a.out ls -lh
	p->cmd.args[0] = ft_strdup(argv[1]);
	if (argv[2] != NULL)
		p->cmd.args[1] = ft_strdup(argv[2]);

	t_shell *shell = malloc(sizeof(t_shell));
	if (shell == NULL) {
		fprintf(stderr, "Failed to allocate memory for t_shell\n");
		return EXIT_FAILURE;
	}
	init_environment(shell);
	execute(p, shell);

	free(p->cmd.args[0]);
	free(p->cmd.args);
	free(p);
	free(shell);

	return EXIT_SUCCESS;
}
*/
