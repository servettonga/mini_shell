/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_pwd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sehosaf <sehosaf@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 14:08:08 by sehosaf           #+#    #+#             */
/*   Updated: 2024/06/19 20:53:20 by sehosaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

/**
 * @brief The `pwd` command writes the absolute pathname of the current working
 * directory to the standard output.
 */
int	cmd_pwd(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, PATH_MAX) == NULL)
	{
		ft_putendl_fd("minishell: pwd: error", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	return (ft_putendl_fd(cwd, STDOUT_FILENO), EXIT_SUCCESS);
}
