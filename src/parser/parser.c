#include "parser.h"

static void set_pipeline_parameters(t_pipeline *node, t_shell *shell);
static void set_connection_type(t_pipeline *node);
static void drop_outside_quotation(t_pipeline *node);

/*
Main API for parsing part of the programm
Arg:
	line (char *) - prompt text
	env (t_env *) - structure which represent env vars
Return:
	Commands pipeline - structure which describes what should be executet

1) Split string into tokens
2) Interpret tokens and devide them by |, && and || operators to commands
3) iterrate through commands:
	3.1) redirections
	3.2) variables
	3.3) wildcard
	3.4) drop quotation markss

Undefined behavior:
- multiple redirections to the same command, eg. `cmd >1.out >>2.out` or `cmd <1.txt <<LIM`
*/
t_pipeline *parse(char *line, t_shell *shell)
{
	t_pipeline   *res;
	char		**tokens;

	tokens = split_line(line);
	res = create_default_pipeline_node();
	if (!tokens || !res)
		return (NULL);
	split_tokens_per_command(res, tokens);
	free_split(tokens);
	set_pipeline_parameters(res, shell);
	return (res);
}

static void set_pipeline_parameters(t_pipeline *node, t_shell *shell)
{
	t_pipeline   *tmp;

	tmp = NULL;
	while (node)
	{
		node->prev = tmp;
		if (!node->cmd.args || !node->cmd.args[0])
			break;
		set_connection_type(node);
		set_redirections(node);
		replace_vars(node, shell);
		replace_wildcards(node);
		drop_outside_quotation(node);
		tmp = node;
		node = node->next;
	}
}

static void set_connection_type(t_pipeline *node)
{
	if (!ft_memcmp(node->cmd.args[0], "|", 2))
	{
		node->cmd.connection_type = CON_PIPE;
		if (node->prev && node->prev->cmd.connection_type == CON_NONE)
			node->prev->cmd.connection_type = CON_PIPE;
	}
	else if (!ft_memcmp(node->cmd.args[0], "||", 3))
		node->cmd.connection_type = CON_OR;
	else if (!ft_memcmp(node->cmd.args[0], "&&", 3))
		node->cmd.connection_type = CON_AND;
	else
		return ;
	remove_cmd_arg(node, 0);
}

static void drop_outside_quotation(t_pipeline *node)
{
	int i;
	int j;
	char *end;
	char *tmp;

	i = 0;
	while (node->cmd.args[i])
	{
		j = 0;
		while (node->cmd.args[i][j])
		{
			tmp = node->cmd.args[i] + j;
			if (ft_strchr("'\"", *tmp))
			{
				end = ft_strchr(tmp + 1, *tmp);
				if (!end)
					break ;
				ft_memmove(tmp, tmp + 1, end - tmp - 1);
				ft_memmove(end - 1, end + 1, ft_strlen(end + 1) + 1);
				j = (end - node->cmd.args[i]) - 2;
			}
			j++;
		}
		i++;
	}
}
