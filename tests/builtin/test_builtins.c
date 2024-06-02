/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sehosaf <sehosaf@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 15:12:16 by sehosaf           #+#    #+#             */
/*   Updated: 2024/06/02 15:13:21 by sehosaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CUnit/Basic.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "../../src/builtin/cmd_echo.c"
#include "../../src/builtin/cmd_pwd.c"

void	test_cmd_pwd(void)
{
	char	buffer[1024 + 2];
	int		fd;
	ssize_t	num_read;
	char	*newline;
	int		saved_stdout;
	char	expected_output[1024];

	saved_stdout = dup(STDOUT_FILENO);
	fd = open("test_output", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd == -1)
	{
		perror("open");
		return ;
	}
	dup2(fd, STDOUT_FILENO);
	CU_ASSERT_EQUAL(cmd_pwd(), EXIT_SUCCESS);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdout);
	close(fd);
	fd = open("test_output", O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		return ;
	}
	num_read = read(fd, buffer, sizeof(buffer));
	if (num_read == -1)
	{
		perror("read");
		close(fd);
		return ;
	}
	buffer[num_read] = '\0';
	close(fd);
	newline = strchr(buffer, '\n');
	if (newline)
		*newline = '\0';
	getcwd(expected_output, sizeof(expected_output));
	printf("Expected output: %s\n", expected_output);
	CU_ASSERT_STRING_EQUAL(buffer, expected_output);
	remove("test_output");
}

void	test_cmd_echo(void)
{
	const char	*args1[] = {"echo", "Hello,", "World!", NULL};
	const char	*args2[] = {"echo", "-n", "Hello,", "World!", NULL};
	const char	*args3[] = {"echo", NULL};
	const char	*args4[] = {NULL};

	CU_ASSERT_EQUAL(cmd_echo(args1), EXIT_SUCCESS);
	CU_ASSERT_EQUAL(cmd_echo(args2), EXIT_SUCCESS);
	CU_ASSERT_EQUAL(cmd_echo(args3), EXIT_SUCCESS);
	CU_ASSERT_EQUAL(cmd_echo(args4), EXIT_FAILURE);
}

int	main(void)
{
	CU_pSuite	pSuite;

	pSuite = NULL;
	if (CUE_SUCCESS != CU_initialize_registry())
		return (CU_get_error());
	pSuite = CU_add_suite("builtin_command_test_suite", 0, 0);
	if (NULL == pSuite)
	{
		CU_cleanup_registry();
		return (CU_get_error());
	}
	if ((NULL == CU_add_test(pSuite, "test_cmd_echo", test_cmd_echo))
		|| (NULL == CU_add_test(pSuite, "test_cmd_pwd", test_cmd_pwd)))
	{
		CU_cleanup_registry();
		return (CU_get_error());
	}
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();
	return (CU_get_error());
}
