#include "minishell.h"

int main(void)
{
    char * line;
    t_pipeline *p;
    t_shell shell;

    init_environment(&shell);
    while (1)
    {
		interactive_signal_handlers();
        line = readline(PROMPT);
		non_interactive_signal_handlers();
        if (!line)
            exit(EXIT_SUCCESS);
        if (!*line)
        {
            free(line);
            continue ;
        }
        add_history(line);
        p = parse(line, &shell);
        free(line);
        if (validate_pipeline(p) == EXIT_SUCCESS)
            execute(p, &shell);
        free_pipeline(p);
    }
}