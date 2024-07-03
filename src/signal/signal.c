/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sehosaf <sehosaf@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 20:54:21 by sehosaf           #+#    #+#             */
/*   Updated: 2024/07/03 11:11:18 by sehosaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"

/**
 * @brief Handles signals in non-interactive mode.
 * @note Replaces SIGQUIT handler with SIG_IGN.
 * @note Ignores SIGINT signal and prints a newline character.
 * @note SIGINT and SIGQUIT are blocked while the signal handler is running.
 */
void	non_interactive_signal_handlers(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sigemptyset(&sa_int.sa_mask);
	sigemptyset(&sa_quit.sa_mask);
	sigaddset(&sa_int.sa_mask, SIGINT);
	sigaddset(&sa_quit.sa_mask, SIGQUIT);
	sa_int.sa_handler = print_newline;
	sa_quit.sa_handler = SIG_IGN;
	sa_int.sa_flags = 0;
	sa_quit.sa_flags = 0;
	if (sigaction(SIGINT, &sa_int, NULL))
		perror("minishell: sigaction: SIGINT ");
	if (sigaction(SIGQUIT, &sa_quit, NULL))
		perror("minishell: sigaction: SIGQUIT ");
}

/**
 * @brief Handles signals in interactive mode.
 * @note This function sets the signal handler for SIGINT to reset the prompt.
 */
void	interactive_signal_handlers(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sigemptyset(&sa_int.sa_mask);
	sigemptyset(&sa_quit.sa_mask);
	sigaddset(&sa_int.sa_mask, SIGINT);
	sigaddset(&sa_quit.sa_mask, SIGQUIT);
	sa_int.sa_handler = sigint_handler;
	sa_quit.sa_handler = SIG_IGN;
	sa_int.sa_flags = 0;
	sa_quit.sa_flags = 0;
	if (sigaction(SIGINT, &sa_int, NULL))
		perror("minishell: sigaction: SIGINT ");
	if (sigaction(SIGQUIT, &sa_quit, NULL))
		perror("minishell: sigaction: SIGQUIT ");
}

/**
 * @note It will interrupt the shell and print a newline character.
 */
void	sigint_handler(int signum)
{
	(void)signum;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/**
 * @note SIGINT is ignored by printing a newline character while
 * in non-interactive mode.
 */
void	print_newline(int signal)
{
	(void)signal;
	rl_on_new_line();
}
