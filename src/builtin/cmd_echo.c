/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sehosaf <sehosaf@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 13:23:46 by sehosaf           #+#    #+#             */
/*   Updated: 2024/06/03 21:22:50 by sehosaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*concat_and_free_old(char *old_str, const char *str_to_add)
{
	char	*new_str;

	new_str = ft_strjoin(old_str, str_to_add);
	free(old_str);
    return (new_str);
}

static char	*concat_args(const char **args, bool n_flag)
{
    int	i;

    i = 1;
    if (n_flag)
        i++;
    char *output = ft_strdup("");
    while (args[i])
    {
        output = concat_and_free_old(output, args[i]);
        if (args[i + 1])
            output = concat_and_free_old(output, " ");
		i++;
    }
    if (!n_flag)
        output = concat_and_free_old(output, "\n");
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
