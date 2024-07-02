/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_unset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sehosaf <sehosaf@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 19:11:53 by sehosaf           #+#    #+#             */
/*   Updated: 2024/06/19 20:53:25 by sehosaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

/**
 * @brief The `unset` command deletes the environment variables.
 * @param shell The shell structure.
 * @param key The key of the environment variable to delete.
*/
void	cmd_unset(t_shell *shell, const char *key)
{
	t_env	*current;

	current = shell->env;
	while (current != NULL)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			free(current->value);
			current->value = NULL;
			return ;
		}
		current = current->next;
	}
}
