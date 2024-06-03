/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sehosaf <sehosaf@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 20:21:32 by sehosaf           #+#    #+#             */
/*   Updated: 2024/06/03 19:06:46 by sehosaf          ###   ########.fr       */
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

/**
 * @brief The `env` command prints the environment variables.
 * @details The variable environ points to an array of pointers to strings called
 * the "environment". The last pointer in this array has the value NULL.
 * @ref https://linux.die.net/man/7/environ
 * @param shell The shell structure.
 */
int	cmd_env(t_shell *shell)
{
	t_env	*env;

	env = shell->env;
	while (env)
	{
		ft_putstr_fd(env->key, STDOUT_FILENO);
		ft_putchar_fd('=', STDOUT_FILENO);
		ft_putendl_fd(env->value, STDOUT_FILENO);
		env = env->next;
	}
	return (EXIT_SUCCESS);
}
