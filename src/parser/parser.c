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

Example:
Line: <infile1 cmd1 arg1 "arg2 $VARIABLE" --arg3 | cmd2 arg1 > outfile1 || cmd3 '$FOO $BAR'
Splitted into tokens: 
	- <infile1
	- cmd1
	- arg1
	- "arg2 $VARIABLE"
	- --arg3
	- |
	- cmd2
	- arg1
	- >
	- outfile1
	- ||
	- cmd3
	- '$FOO $BAR'
Split tokens by commands:
	1)
		<infile1
		cmd1
		arg1
		"arg2 $VARIABLE"
		--arg3
	2)
		|
		cmd2
		arg1
		>
		outfile1
	3)
		cmd3
		'$FOO $BAR'

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
		set_connection_type(node); // TODO implement `void set_connection_type(t_pipeline *node)`
		set_redirections(node); // TODO implement `void set_redirections(t_pipeline *node)`
		replace_vars(node); // TODO implement `void replace_vars(t_pipeline *node)`
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

void set_redirections(t_pipeline *node)
{
	int i;

	i = 0;
	while (node->cmd.args[i])
	{
		if (node->cmd.args[i][0] == '<')
		{
			clean_other_input_type(node, node->cmd.args[i][1] == '<');
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

void	set_redirection_field(t_pipeline *node, int i, char **str_field, int checked_char)
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

void clean_other_input_type(t_pipeline *node); // TODO maybe instead of clean set variable which type was last

void remove_cmd_arg(t_pipeline *node, int ind)
{
	free(node->cmd.args[ind]);
	while (node->cmd.args[ind])
	{
		node->cmd.args[ind] = node->cmd.args[ind + 1];
		ind++;
	}
}
