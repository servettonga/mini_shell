/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoroz <dmoroz@student.42warsaw.pl>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 18:44:13 by sehosaf           #+#    #+#             */
/*   Updated: 2024/06/03 14:12:15 by dmoroz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <bits/waitflags.h>
# include <signal.h>
# include <dirent.h>
# include <errno.h>
# include <stdbool.h>
# include "libft.h"

# define PATH_MAX 4096

/*
t_command struct represent command which has to be executet
Fields:
    args (char **) - command name/path and it's arguments
    connection_type (char *) - the way commend is connected to previous one;
        possible types: "|" - pipe, "&&" - and operator, "||" - or operator, NULL - first command
    is_heredoc (int) - flag, True if input redirection has to be from here_doc
    limiter (char *) - limiter string for here_doc
    infile (char *) - file name of redirected input source, if should be; NULL otherwise
    outfile (char *) - file name of redirected output, if should be; NULL otherwise
    outfile_append_mode (int) - flag, True if output redirection has to be in append mode;
        ignore if outfile is NULL
*/

typedef enum e_connection {
    CON_NONE,
    CON_PIPE,
    CON_OR,
    CON_AND
} t_connection;

typedef struct s_command
{
    char    **args;
    t_connection connection_type;
    int     is_heredoc;
    char    *limiter;
    char    *infile;
    char    *outfile;
    int     outfile_append_mode;
}       t_command;

typedef struct s_pipeline
{
    t_command  cmd;
    struct s_pipeline *next;
}       t_pipeline;

t_pipeline *parse(char *line);

// utils
char *get_varval(char *name); // TODO implement in env section
void free_split(char **split);
int get_split_size(char **split);
int	ft_isspace(int c);

// TODO: structure suggestion for environment variables
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_shell
{
	t_env	*env;
	char	*cwd;
}	t_shell;

#endif

