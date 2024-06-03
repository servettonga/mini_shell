/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sehosaf <sehosaf@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 20:21:32 by sehosaf           #+#    #+#             */
/*   Updated: 2024/06/03 12:33:47 by sehosaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief The `env` command prints the environment variables.
 * @details The variable environ points to an array of pointers to strings called
 * the "environment". The last pointer in this array has the value NULL.
 * @ref https://linux.die.net/man/7/environ
 */
int	cmd_env(void)
{
	extern char	**environ;
	int			i;

	if (environ == NULL)
		return (EXIT_FAILURE);
	i = 0;
	while (environ[i])
		ft_putendl_fd(environ[i++], STDOUT_FILENO);
	return (EXIT_SUCCESS);
}
