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
    char        **tokens;

    tokens = split_line(line);
    if (!tokens)
        return (NULL);
    res = create_defalt_pipeline_node();
    split_tokens_per_command(res, tokens);
    set_pipeline_parameters(res);
    free_split(tokens); // TODO implement `void free_split(char **split)`
    return (res);
}

void set_pipeline_parameters(t_pipeline *start)
{
    while (*start)
    {
        set_connection_type(start); // TODO implement `void set_connection_type(t_pipeline *node)`
        set_redirections(start); // TODO implement `void set_redirections(t_pipeline *node)`
        replace_vars(start); // TODO implement `void replace_vars(t_pipeline *node)`
        replace_wildcard(start); // TODO implement `void replace_wildcard(t_pipeline *node)`
        start = start->next;
    }
}
