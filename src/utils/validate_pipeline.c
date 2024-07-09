#include "minishell.h"

char *get_conn_type_name(t_connection c);

int validate_pipeline(t_pipeline *p)
{
    t_pipeline *current;
    t_command *cmd;
    
    current = p;
    while (current) {
        cmd = &current->cmd;
        if (!cmd->args || !cmd->args[0])
        {
            if (current->next)
            {
                ft_putendl_fd("minishell: syntax error near unexpected token `", STDERR_FILENO);
                ft_putendl_fd(get_conn_type_name(current->next->cmd.connection_type), STDERR_FILENO);
                ft_putendl_fd("'", STDERR_FILENO);
            }
            else
                ft_putendl_fd("minishell: syntax error: unexpected end of file", STDERR_FILENO);
		    return (EXIT_FAILURE);
        }
        current = current->next;
    }
    return (EXIT_SUCCESS);
}

char *get_conn_type_name(t_connection c)
{
    if (c == CON_PIPE)
        return ("|");
    if (c == CON_OR)
        return ("||");
    if (c == CON_AND)
        return ("&&");
    return ("NONE");
}
