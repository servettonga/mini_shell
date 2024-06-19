/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sehosaf <sehosaf@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 21:04:06 by sehosaf           #+#    #+#             */
/*   Updated: 2024/06/19 21:30:27 by sehosaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

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
