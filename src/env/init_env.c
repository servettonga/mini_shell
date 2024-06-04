/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sehosaf <sehosaf@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 21:34:34 by sehosaf           #+#    #+#             */
/*   Updated: 2024/06/03 21:38:16 by sehosaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env	*create_env_node(char **key_value)
{
	t_env	*new_node;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (new_node == NULL)
		return (NULL);
	new_node->key = ft_strdup(key_value[0]);
	new_node->value = ft_strdup(key_value[1]);
	if (new_node->key == NULL || new_node->value == NULL)
	{
		free(new_node->key);
		free(new_node->value);
		free(new_node);
		return (NULL);
	}
	new_node->next = NULL;
	return (new_node);
}

static int	handle_errors(t_env *new_node, char **key_value)
{
	int ret;

	ret = EXIT_SUCCESS;
	if (key_value == NULL || key_value[0] == NULL || key_value[1] == NULL)
	{
		perror("ft_split");
		ret = EXIT_FAILURE;
	}
	if (new_node == NULL)
	{
		perror("malloc");
		ret = EXIT_FAILURE;
	}
	if (key_value)
	{
		free(key_value[0]);
		free(key_value[1]);
		free(key_value);
	}
	return (ret);
}

static t_env	*add_env_node(t_env *last_n, t_env *new_n, char **key_val)
{
	free(key_val[0]);
	free(key_val[1]);
	free(key_val);
	if (last_n == NULL)
		return (new_n);
	last_n->next = new_n;
	return (last_n);
}

static char	**split_key_value(const char *s, char c)
{
	char	**result;
	char	*index;

	result = (char **)malloc(sizeof(char *) * 3);
	if (!result)
		return (NULL);
	index = ft_strchr(s, c);
	if (index)
	{
		result[0] = ft_substr(s, 0, index - s);
		result[1] = ft_strdup(index + 1);
	}
	else
	{
		result[0] = ft_strdup(s);
		result[1] = NULL;
	}
	result[2] = NULL;
	return (result);
}

/**
 * @brief Initializes the environment variables.
 * @details The variable environ points to an array of pointers to strings called
 * the "environment". The last pointer in this array has the value NULL.
 * @param shell The shell structure.
*/
int	init_environment(t_shell *shell)
{
	extern char	**environ;
	char		**key_value;
	t_env		*new_node;
	t_env		*last_node;
	int			i;

	if (shell && shell->env == NULL)
	{
		i = 0;
		while (environ[i])
		{
			key_value = split_key_value(environ[i], '=');
			new_node = create_env_node(key_value);
			if (handle_errors(new_node, key_value) == EXIT_FAILURE)
				return (EXIT_FAILURE);
			if (i == 0)
				shell->env = new_node;
			else
				last_node->next = new_node;
			last_node = new_node;
			i++;
		}
	}
	return (EXIT_SUCCESS);
}
