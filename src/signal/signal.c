/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sehosaf <sehosaf@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 20:54:21 by sehosaf           #+#    #+#             */
/*   Updated: 2024/07/01 22:03:18 by sehosaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handles the SIGINT signal. This is typically initiated by
 * pressing Ctrl+C.
 * @note It will interrupt the shell and print a newline character.
 */
void	sigint_handler(int signum)
{
	(void)signum;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_redisplay();
}

/**
 * @brief Handles the SIGQUIT signal. This is typically initiated by
 * pressing Ctrl+\.
 * @note It will ignore the signal.
 */
void	sigquit_handler(int signum)
{
	(void)signum;
	return ;
}

/**
 * @brief Sets the signal handlers for SIGINT and SIGQUIT.
 */
void	set_signal_handlers(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, sigquit_handler);
}
