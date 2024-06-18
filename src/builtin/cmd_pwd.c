/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_pwd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sehosaf <sehosaf@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 14:08:08 by sehosaf           #+#    #+#             */
/*   Updated: 2024/06/12 11:06:21 by sehosaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
