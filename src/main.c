#include "minishell.h"

int main(void)
{
    char * line;
    t_pipeline *p;
    t_shell shell;

    init_environment(&shell);
    non_interactive_signal_handlers();
    while (1)
    {
        line = readline(PROMPT);
        if (!line)
            exit(EXIT_SUCCESS);
        if (!*line)
        {
            free(line);
            continue ;
        }
        add_history(line);
        p = parse(line, shell.env);
        free(line);
        if (validate_pipeline(p) == EXIT_SUCCESS)
            execute(p, &shell);
        free_pipeline(p);
    }
}