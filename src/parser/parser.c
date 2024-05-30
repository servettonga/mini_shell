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
	free_split(tokens); // TODO implement `void free_split(char **split)`
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
		replace_wildcard(node); // TODO implement `void replace_wildcard(t_pipeline *node)`
		node = node->next;
	}
}

void set_connection_type(t_pipeline *node)
{
	if (ft_memcmp(node->cmd.args[0], "|", 2) 
		|| ft_memcmp(node->cmd.args[0], "||", 3)
		|| ft_memcmp(node->cmd.args[0], "&&", 3))
	{
		node->cmd.connection_type = ft_strdup(node->cmd.args[0]);
		remove_cmd_arg(node, 0);
	}
}

void replace_vars(t_pipeline *node)
{
	int i;
	char *start;
	char *var_value;
	char *var_name;
	char *expanded;

	i = -1;
	while (node->cmd.args[++i])
	{
		start = ft_strchr(node->cmd.args[i], '$');
		if (!start)
			continue ;
		if (!validate_var_expansion(node->cmd.args[i], start)) // TODO implement validate_var_expansion;
			continue ;
		var_name = get_var_name(start); // TODO implement char *get_var_name(char *dollar);
		var_value = get_varval(var_name); // TODO implement char *get_varval(char *name);
		expanded = malloc(ft_strlen(node->cmd.args[i]) - ft_strlen(var_name) + ft_strlen(var_value));
		ft_memcpy(expanded, node->cmd.args[i], node->cmd.args[i] - start);
		ft_memcpy(expanded + (node->cmd.args[i] - start), var_value, ft_strlen(var_value));
		ft_memcpy(expanded + (node->cmd.args[i] - start) + ft_strlen(var_value), node->cmd.args[i], ft_strlen(node->cmd.args[i]) - ft_strlen(var_name) - (node->cmd.args[i] - start));
		expanded[ft_strlen(node->cmd.args[i]) - ft_strlen(var_name) + ft_strlen(var_value)] = 0;
		free(node->cmd.args[i]);
		node->cmd.args[i] = expanded;
		i--;
	}
}

char *get_var_name(char *dollar)
{}
