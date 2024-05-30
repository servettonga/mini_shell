#include "parser.h"

static void	set_redirection_field(t_pipeline *node, int i, char **str_field, int checked_char);

void set_redirections(t_pipeline *node)
{
	int i;

	i = 0;
	while (node->cmd.args[i])
	{
		if (node->cmd.args[i][0] == '<')
		{
			if (node->cmd.args[i][1] == '<')
			{
				node->cmd.is_heredoc = 1;
				set_redirection_field(node, i, &node->cmd.limiter, 2);
			}
			else
				set_redirection_field(node, i, &node->cmd.infile, 1);
		}
		else if (node->cmd.args[i][0] == '>')
		{
			if (node->cmd.args[i][1] == '>')
				node->cmd.outfile_append_mode = 1;
			set_redirection_field(node, i, &node->cmd.outfile, 1 + node->cmd.outfile_append_mode);
		}
		else
			i++;
	}
}

static void	set_redirection_field(t_pipeline *node, int i, char **str_field, int checked_char)
{
	char *arg;

	arg = node->cmd.args[i];
	if (arg[checked_char] == 0)
	{
		*str_field = ft_strdup(node->cmd.args[i + 1]);
		remove_cmd_arg(node, i + 1);
	}
	else
		*str_field = ft_substr(arg, checked_char, ft_strlen(arg));
	remove_cmd_arg(node, i);
}
