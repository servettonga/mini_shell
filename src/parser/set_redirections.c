/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_redirections.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sehosaf <sehosaf@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 20:28:38 by sehosaf           #+#    #+#             */
/*   Updated: 2024/07/17 09:44:43 by sehosaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static void	set_redir_field(t_pipeline *nd, int i, char **s_field, int ck_char);

/*
	Finds tokens which define redirection, set respective t_command values
	and drop such tokens. Designed to work with cases where there is only
	one type of in and out redirections.
*/
void	set_redirections(t_pipeline *node)
{
	int	i;

	i = 0;
	while (node->cmd.args[i])
	{
		if (node->cmd.args[i][0] == '<')
		{
			if (node->cmd.args[i][1] == '<')
			{
				node->cmd.is_heredoc = 1;
				set_redir_field(node, i, &node->cmd.limiter, 2);
			}
			else
				set_redir_field(node, i, &node->cmd.infile, 1);
		}
		else if (node->cmd.args[i][0] == '>')
		{
			if (node->cmd.args[i][1] == '>')
				node->cmd.ap_mode = 1;
			set_redir_field(node, i, &node->cmd.outfile, 1 + node->cmd.ap_mode);
		}
		else
			i++;
	}
}

static void	set_redir_field(t_pipeline *nd, int i, char **s_field, int ck_char)
{
	char	*arg;

	arg = nd->cmd.args[i];
	if (arg[ck_char] == 0)
	{
		*s_field = ft_strdup(nd->cmd.args[i + 1]);
		remove_cmd_arg(nd, i + 1);
	}
	else
		*s_field = ft_substr(arg, ck_char, ft_strlen(arg));
	remove_cmd_arg(nd, i);
}
