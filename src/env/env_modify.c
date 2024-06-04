/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_modify.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sehosaf <sehosaf@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 21:29:40 by sehosaf           #+#    #+#             */
/*   Updated: 2024/06/04 21:37:41 by sehosaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	free_key_val(char **key_val, bool success);
static int	overwrite_var(t_env *existing_node, char **key_val);

/**
 * @brief Creates a new environment variable.
 * @details If the variable already exists, it will be overwritten.
 * @param env The environment variable list of the shell structure
 * @param key_value The key and value of the environment variable as a string.
*/
int	create_env_var(t_env **env, const char *key_value)
{
	t_env	*new_node;
	t_env	*last_node;
	char	**key_val;
	t_env	*existing_node;

	key_val = split_key_value(key_value, '=');
	if (key_val == NULL)
		return (EXIT_FAILURE);
	if (key_val[0] == NULL || key_val[1] == NULL)
		return (free_key_val(key_val, false));
	existing_node = get_env_node(*env, key_val[0]);
	if (existing_node)
		return (overwrite_var(existing_node, key_val));
	new_node = create_env_node(key_val);
	if (new_node == NULL)
		return (EXIT_FAILURE);
	last_node = get_last_node(*env);
	if (last_node == NULL)
		*env = new_node;
	else
		last_node->next = new_node;
	return (free_key_val(key_val, true));
}

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

static int	free_key_val(char **key_val, bool success)
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

static int	overwrite_var(t_env *existing_node, char **key_val)
{
	if (existing_node->value && key_val)
	{
		free(existing_node->value);
		existing_node->value = key_val[1];
		free(key_val[0]);
		free(key_val);
		return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}
