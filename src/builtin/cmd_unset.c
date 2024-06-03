/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_unset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sehosaf <sehosaf@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 19:11:53 by sehosaf           #+#    #+#             */
/*   Updated: 2024/06/03 19:16:32 by sehosaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <errno.h>
#include <stdio.h>

// TODO: structure suggestion for environment variables
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_shell
{
	t_env	*env;
	char	*cwd;
}	t_shell;

/**
 * @brief The `unset` command deletes the environment variables.
 * @param shell The shell structure.
 * @param key The key of the environment variable to delete.
*/
void	cmd_unset(t_shell *shell, char *key)
{
	t_env	*prev;
	t_env	*current;

	current = shell->env;
	prev = NULL;
	while (current != NULL)
	{
		if (strcmp(current->key, key) == 0)
		{
			if (prev == NULL)
				shell->env = current->next;
			else
				prev->next = current->next;
			free(current->key);
			free(current->value);
			free(current);
			printf("Deleted %s\n", key);
			return ;
		}
		prev = current;
		current = current->next;
	}
	printf("unset: %s: not found\n", key);
}
