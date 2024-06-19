#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"
# include "libft.h"

char		**split_line(char *line);
void		split_tokens_per_command(t_pipeline *start, char **tokens);
t_pipeline	*create_default_pipeline_node(void);
void		set_redirections(t_pipeline *node);
void		remove_cmd_arg(t_pipeline *node, int ind);
void		replace_vars(t_pipeline *node, t_env *env);
void		replace_wildcards(t_pipeline *node);

#endif
