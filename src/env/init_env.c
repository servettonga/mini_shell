/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sehosaf <sehosaf@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 19:00:23 by sehosaf           #+#    #+#             */
/*   Updated: 2024/06/03 19:07:07 by sehosaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

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

static t_env	*create_env_node(char **key_value)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (new_node == NULL)
		return (NULL);
	new_node->key = ft_strdup(key_value[0]);
	new_node->value = ft_strdup(key_value[1]);
	if (new_node->key == NULL || new_node->value == NULL)
		return (free(new_node), NULL);
	new_node->next = NULL;
	return (new_node);
}

static int	handle_split_error(char **key_value)
{
	if (key_value == NULL || key_value[0] == NULL || key_value[1] == NULL)
	{
		if (key_value)
		{
			free(key_value[0]);
			free(key_value[1]);
			free(key_value);
		}
		return (perror("ft_split"), EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static int	handle_node_creation_error(t_env *new_node, char **key_value)
{
	if (new_node == NULL)
	{
		free(key_value[0]);
		free(key_value[1]);
		free(key_value);
		return (perror("malloc"), EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
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

	if (shell->env == NULL)
	{
		i = 0;
		last_node = NULL;
		while (environ[i])
		{
			key_value = ft_split(environ[i], '=');
			if (handle_split_error(key_value) == EXIT_FAILURE)
				return (EXIT_FAILURE);
			new_node = create_env_node(key_value);
			if (handle_node_creation_error(new_node, key_value) == EXIT_FAILURE)
				return (EXIT_FAILURE);
			last_node = add_env_node(last_node, new_node, key_value);
			if (i == 0)
				shell->env = new_node;
			i++;
		}
	}
	return (EXIT_SUCCESS);
}
