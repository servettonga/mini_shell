/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_pwd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sehosaf <sehosaf@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 14:08:08 by sehosaf           #+#    #+#             */
/*   Updated: 2024/06/02 15:10:32 by sehosaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief The `pwd` command writes the absolute pathname of the current working
 * directory to the standard output.
 */
int	cmd_pwd(void)
{
	char	buf[1024];

	if (getcwd(buf, sizeof(buf)) == NULL)
	{
		perror("pwd");
		return (EXIT_FAILURE);
	}
	ft_putendl_fd(buf, STDOUT_FILENO);
	return (EXIT_SUCCESS);
}
