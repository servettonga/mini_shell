/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_unset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sehosaf <sehosaf@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 19:11:53 by sehosaf           #+#    #+#             */
/*   Updated: 2024/06/12 10:38:33 by sehosaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief The `unset` command deletes the environment variables.
 * @param shell The shell structure.
 * @param key The key of the environment variable to delete.
*/
void	cmd_unset(t_shell *shell, const char *key)
{
	t_env	*prev;
	t_env	*current;

	current = shell->env;
	prev = NULL;
	while (current != NULL)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (prev == NULL)
				shell->env = current->next;
			else
				prev->next = current->next;
			free(current->value);
			current->value = NULL;
			return ;
		}
		prev = current;
		current = current->next;
	}
	printf("unset: %s: not found\n", key);
}
