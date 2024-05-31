#include "parser.h"

/*
Main API for parsing part of the programm
Arg:
	line (char *) - prompt text
Return:
	Commands pipeline - structure which describes what should be executet

1) Split string into tokens
2) Interpret tokens and devide them by |, && and || operators to commands
3) iterrate through commands:
	3.1) redirections
	3.2) variables
	3.3) drop quotations
	3.4) wildcard

Undefined behavior:
- multiple redirections to the same command, eg. `cmd >1.out >>2.out` or `cmd <1.txt <<LIM`
*/
t_pipeline *parse(char *line)
{
	t_pipeline   *res;
	char		**tokens;

	tokens = split_line(line);
	if (!tokens)
		return (NULL);
	res = create_defalt_pipeline_node();
	split_tokens_per_command(res, tokens);
	free_split(tokens);
	set_pipeline_parameters(res);
	return (res);
}

void set_pipeline_parameters(t_pipeline *node)
{
	while (node)
	{
		if (!node->cmd.args || !node->cmd.args[0])
			break;
		set_connection_type(node);
		set_redirections(node);
		replace_vars(node);
		replace_wildcards(node);
		node = node->next;
	}
}

void set_connection_type(t_pipeline *node)
{
	if (ft_memcmp(node->cmd.args[0], "|", 2))
		node->cmd.connection_type = CON_PIPE;
	else if (ft_memcmp(node->cmd.args[0], "||", 3))
		node->cmd.connection_type = CON_OR;
	else if (ft_memcmp(node->cmd.args[0], "&&", 3))
		node->cmd.connection_type = CON_AND;
	else
		return ;
	remove_cmd_arg(node, 0);
}
