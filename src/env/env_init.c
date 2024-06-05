/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sehosaf <sehosaf@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 21:34:34 by sehosaf           #+#    #+#             */
/*   Updated: 2024/06/05 09:28:14 by sehosaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int		handle_errors(t_env *new_node, char **key_value);
static t_env	*add_env_node(t_env *last_n, t_env *new_n, char **key_val);

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

static int	handle_errors(t_env *new_node, char **key_value)
{
	int	ret;

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
