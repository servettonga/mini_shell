#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

char        **split_line(line);
void        split_tokens_per_command(t_pipeline *start, char **tokens);
t_pipeline  *create_defalt_pipeline_node(void);

#endif