/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sehosaf <sehosaf@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 09:32:25 by sehosaf           #+#    #+#             */
/*   Updated: 2024/07/17 10:20:05 by sehosaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_pipeline(t_pipeline *p)
{
	t_pipeline	*tmp;

	while (p)
	{
		free_split(p->cmd.args);
		free(p->cmd.limiter);
		free(p->cmd.outfile);
		free(p->cmd.infile);
		tmp = p;
		p = p->next;
		free(tmp);
	}
}
