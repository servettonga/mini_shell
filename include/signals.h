/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sehosaf <sehosaf@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 09:03:18 by sehosaf           #+#    #+#             */
/*   Updated: 2024/07/03 11:10:35 by sehosaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# ifndef _POSIX_C_SOURCE
#  define _POSIX_C_SOURCE 200809L
# endif

# include <features.h>
# include <signal.h>
# include "minishell.h"

//		** signal handlers **
void	print_newline(int signum);
void	sigint_handler(int signum);

#endif
