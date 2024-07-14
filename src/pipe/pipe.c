/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sehosaf <sehosaf@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 16:56:55 by sehosaf           #+#    #+#             */
/*   Updated: 2024/07/12 16:56:56 by sehosaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

int create_pipes(t_pipeline *p)
{
	int			pipefd[1024][2];
	t_pipeline	*current;
	int 		i;

	current = p;
	if (!p)
		return (EXIT_FAILURE);
	i = 0;
	while (current)
	{
		if (current->cmd.connection_type == CON_PIPE)
		{
			if (pipe(pipefd[i]) == -1)
				return (perror("minishell: pipe error: "), EXIT_FAILURE);
			current->fd_in = pipefd[i][0];
			current->fd_out = pipefd[i][1];
			i++;
		}
		current = current->next;
	}
	return (EXIT_SUCCESS);
}

void	close_pipes(t_pipeline *p, t_pipeline *ignore)
{
	t_pipeline	*current;

	current = p;
	while (current)
	{
		if (current != ignore)
		{
			close(current->fd_in);
			close(current->fd_out);
		}
		current = current->next;
	}
}