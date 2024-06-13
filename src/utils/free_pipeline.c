#include "minishell.h"

void free_pipeline(t_pipeline * p)
{
    t_pipeline * tmp;

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
