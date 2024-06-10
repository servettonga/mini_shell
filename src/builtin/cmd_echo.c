/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sehosaf <sehosaf@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 13:23:46 by sehosaf           #+#    #+#             */
/*   Updated: 2024/06/02 13:27:02 by sehosaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdbool.h>

static char	*concat_args(const char **args, bool n_flag)
{
	char	*output;
	char	*temp;
	int		i;

	i = 1;
	if (n_flag)
		i++;
	output = ft_strdup("");
	while (args[i])
	{
		temp = ft_strjoin(output, args[i]);
		free(output);
		output = temp;
		if (args[i + 1])
			output = ft_strjoin(output, " ");
		i++;
	}
	if (!n_flag)
		output = ft_strjoin(output, "\n");
	return (output);
}

/**
 * @brief The `echo` command writes any specified operands,
 * separated by single blank (`' '`) characters and
 * followed by a newline (`'\\n'`) character, to the standard output.
 * @param args The arguments passed to the echo command
 * including the command itself.
*/
int	cmd_echo(const char **args)
{
	bool	n_flag;
	char	*output;

	n_flag = false;
	if (args == NULL || *args == NULL)
		return (EXIT_FAILURE);
	if (args[1] && ft_strncmp(args[1], "-n", 2) == 0)
		n_flag = true;
	output = concat_args(args, n_flag);
	ft_putstr_fd(output, STDOUT_FILENO);
	free(output);
	return (EXIT_SUCCESS);
}
