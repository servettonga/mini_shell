#include "parser.h"

static void fill_node_with_tokens(t_pipeline *start, char **tokens, int first, int curr);

/*
Creates node for each command and fills it args with relevand tokens.
*/
void split_tokens_per_command(t_pipeline *start, char **tokens)
{
    int first;
    int curr;

    first = 0;
    curr = 0;
    while (tokens[curr])
    {
        if (!ft_memcmp(tokens[curr], "|", 2)
            || !ft_memcmp(tokens[curr], "||", 3)
            || !ft_memcmp(tokens[curr], "&&", 3))
        {
            fill_node_with_tokens(start, tokens, first, curr);
            start->next = create_default_pipeline_node();
            start = start->next;
            first = curr;
        }
        curr++;
    }
    fill_node_with_tokens(start, tokens, first, curr);
}

static void fill_node_with_tokens(t_pipeline *start, char **tokens, int first, int curr)
{
    int i;

    start->cmd.args = malloc((curr - first + 1) * sizeof(char *));
    if (!start->cmd.args)
        return ;
    i = 0;
    while (i < curr - first)
    {
        start->cmd.args[i] = ft_strdup(tokens[first + i]);
        i++;
    }
    start->cmd.args[curr - first] = NULL;
}
