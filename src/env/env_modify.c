/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_modify.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sehosaf <sehosaf@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 21:29:40 by sehosaf           #+#    #+#             */
/*   Updated: 2024/06/05 09:48:28 by sehosaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Gets the value of an environment variable.
 * @param env The environment variable list of the shell structure
 * @param key The key of the environment variable
*/
char	*get_env_val(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

/**
 * @brief Sets the value of an environment variable if it exists.
 * @param env The environment variable list of the shell structure
 * @param key The key of the environment variable
 * @param value The value of the environment variable
*/
int	set_env_var(t_env *env, const char *key, const char *value)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
		{
			free(env->value);
			env->value = ft_strdup(value);
			if (env->value == NULL)
				return (EXIT_FAILURE);
			return (EXIT_SUCCESS);
		}
		env = env->next;
	}
	return (EXIT_FAILURE);
}

int	free_key_val(char **key_val, bool success)
{
	if (key_val)
	{
		free(key_val[0]);
		free(key_val[1]);
		free(key_val);
	}
	if (success)
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

int	overwrite_var(t_env *existing_node, char **key_val)
{
	if (existing_node->value && key_val)
	{
		free(existing_node->value);
		existing_node->value = ft_strdup(key_val[1]);
		return (free_key_val(key_val, true));
	}
	return (EXIT_FAILURE);
}

/**
 * @brief Frees the environment variable list.
 * @param env The environment variable list of the shell structure
*/
void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
}
