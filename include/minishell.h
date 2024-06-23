/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sehosaf <sehosaf@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 20:37:44 by sehosaf           #+#    #+#             */
/*   Updated: 2024/06/22 19:16:08 by sehosaf          ###   ########.fr       */
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
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <bits/waitflags.h>
# include <signal.h>
# include <dirent.h>
# include <errno.h>
# include <stdbool.h>
# include <limits.h>
# include "libft.h"

/*
t_command struct represent command which has to be executed
Fields:
	args (char **) - command name/path and it's arguments
	connection_type (char *) - the way commend is connected to previous one;
	possible types: "|" - pipe, "&&" - and operator, "||" -
	or operator, NULL - first command
	is_heredoc (int) - flag, True if input redirection has to be from here_doc
	limiter (char *) - limiter string for here_doc
	infile (char *) - file name of redirected input source,
	if should be; NULL otherwise
	outfile (char *) - file name of redirected output,
	if should be; NULL otherwise
	outfile_append_mode (int) - flag,
	True if output redirection has to be in append mode;
	ignore if outfile is NULL
*/

typedef enum e_connection
{
	CON_NONE,
	CON_PIPE,
	CON_OR,
	CON_AND
}	t_connection;

typedef struct s_command
{
	char			**args;
	t_connection	connection_type;
	int				is_heredoc;
	char			*limiter;
	char			*infile;
	char			*outfile;
	int				outfile_append_mode;
}	t_command;

typedef struct s_pipeline
{
	t_command			cmd;
	struct s_pipeline	*next;
}	t_pipeline;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_shell
{
	t_env	*env;
	int		exit_status;
}	t_shell;

t_pipeline	*parse(char *line, t_env *env);

//			** utility functions **
void		cleanup_and_exit_shell(t_shell *shell);
long		ft_atol(const char *str);
void		free_split(char **split);
int			get_split_size(char **split);
int			ft_isspace(int c);

#endif
