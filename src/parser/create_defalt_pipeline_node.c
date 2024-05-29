#include "parser.h"

t_pipeline *create_defalt_pipeline_node(void)
{
    t_pipeline  *res;

    res = malloc(sizeof(t_pipeline));
    res->next = NULL;
    res->cmd.args = NULL;
    res->cmd.connection_type = NULL;
    res->cmd.is_heredoc = 0;
    res->cmd.limiter = NULL;
    res->cmd.infile = NULL;
    res->cmd.outfile = NULL;
    res->cmd.outfile_append_mode = 0;
    return (res);
}