/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_redirections.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sehosaf <sehosaf@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 20:28:38 by sehosaf           #+#    #+#             */
/*   Updated: 2024/07/18 20:44:17 by sehosaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int	set_redir(t_pipeline *nd, int i, char **s_field, int ck_char);

/*
	Finds tokens which define redirection, set respective t_command values
	and drop such tokens. Designed to work with cases where there is only
	one type of in and out redirections.
*/
int	set_redirections(t_pipeline *node)
{
	int	i;

	i = 0;
	while (node->cmd.args[i])
	{
		if (node->cmd.args[i][0] == '<')
		{
			if (node->cmd.args[i][1] == '<')
			{
				if (set_redir(node, i, &node->cmd.limiter, 2))
					return (1);
			}
			else
				set_redir(node, i, &node->cmd.infile, 1);
		}
		else if (node->cmd.args[i][0] == '>')
		{
			if (node->cmd.args[i][1] == '>')
				node->cmd.ap_mode = 1;
			set_redir(node, i, &node->cmd.outfile, 1 + node->cmd.ap_mode);
		}
		else
			i++;
	}
	return (0);
}

static int	set_redir(t_pipeline *nd, int i, char **s_field, int ck_char)
{
	char	*arg;

	if (!nd->cmd.args[i + 1])
	{
		ft_putstr_fd(ERR_SNX_TOKEN, STDERR_FILENO);
		ft_putendl_fd(nd->cmd.args[i], STDERR_FILENO);
		return (1);
	}
	if (nd->cmd.args[i][1] == '<')
		nd->cmd.is_heredoc = 1;
	arg = nd->cmd.args[i];
	if (arg[ck_char] == 0)
	{
		*s_field = ft_strdup(nd->cmd.args[i + 1]);
		remove_cmd_arg(nd, i + 1);
	}
	else
		*s_field = ft_substr(arg, ck_char, ft_strlen(arg));
	remove_cmd_arg(nd, i);
	return (0);
}
