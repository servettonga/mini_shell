/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sehosaf <sehosaf@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 15:12:16 by sehosaf           #+#    #+#             */
/*   Updated: 2024/06/19 21:30:54 by sehosaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CUnit/Basic.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "minishell.h"
#include "execute.h"

static void	test_cmd_pwd(void)
{
	char	buffer[PATH_MAX + 2];
	int		fd;
	ssize_t	num_read;
	char	*newline;
	int		saved_stdout;
	char	expected_output[PATH_MAX];
	t_shell	*shell;

	shell = (t_shell *)malloc(sizeof(t_shell));
	saved_stdout = dup(STDOUT_FILENO);
	fd = open("test_output", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd == -1)
	{
		perror("open");
		free(shell);
		return ;
	}
	dup2(fd, STDOUT_FILENO);
	CU_ASSERT_EQUAL(cmd_pwd(), EXIT_SUCCESS);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdout);
	close(fd);
	free(shell);
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
	printf("\nExpected cwd: %s\n", expected_output);
	printf("Actual cwd: %s\n", buffer);
	CU_ASSERT_STRING_EQUAL(buffer, expected_output);
	remove("test_output");
}

static void	test_cmd_echo(void)
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

static void test_cmd_unset(void)
{
	t_shell *shell = (t_shell *)malloc(sizeof(t_shell));
	shell->env = NULL;
    init_environment(shell);
    cmd_unset(shell, "PATH");
    t_env *env = shell->env;
    while (env != NULL)
    {
        CU_ASSERT_STRING_NOT_EQUAL(env->key, "PATH");
        env = env->next;
    }
	free_env(shell->env);
	free(shell);
}

static void test_cmd_env(void)
{
	t_shell *shell = (t_shell *)malloc(sizeof(t_shell));
	shell->env = NULL;
    init_environment(shell);
    int saved_stdout = dup(STDOUT_FILENO);
    int fd = open("test_output", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        perror("open");
		free_env(shell->env);
		free(shell);
		return;
	}
    dup2(fd, STDOUT_FILENO);
    cmd_env(shell);
	free_env(shell->env);
	free(shell);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);
    close(fd);
    fd = open("test_output", O_RDONLY);
    if (fd == -1)
    {
        perror("open");
        return;
    }
    char buffer[1024];
    ssize_t num_read = read(fd, buffer, 1024);
    if (num_read == -1)
    {
        perror("read");
        return;
    }
    buffer[num_read] = '\0';
    close(fd);
    CU_ASSERT_PTR_NOT_NULL(strstr(buffer, "HOME="));
    CU_ASSERT_PTR_NOT_NULL(strstr(buffer, "USER="));
	remove("test_output");
}

static void test_cmd_export(void)
{
    t_shell *shell = (t_shell *)malloc(sizeof(t_shell));
    shell->env = NULL;
    init_environment(shell);
    char *new_var = "NEW_VAR=new_value";
    cmd_export(shell, new_var);
    t_env *env_node = get_env_node(shell->env, "NEW_VAR");
    CU_ASSERT_PTR_NOT_NULL(env_node);
    if (env_node != NULL)
        CU_ASSERT_STRING_EQUAL(env_node->value, "new_value");
    char *overwrite_var = "HOME=new_home";
    cmd_export(shell, overwrite_var);
    env_node = get_env_node(shell->env, "HOME");
    CU_ASSERT_PTR_NOT_NULL(env_node);
    if (env_node != NULL)
        CU_ASSERT_STRING_EQUAL(env_node->value, "new_home");
    free_env(shell->env);
    free(shell);
}

int	main(void)
{
	CU_pSuite	pSuite;

	pSuite = NULL;
	if (CUE_SUCCESS != CU_initialize_registry())
		return (CU_get_error());
	pSuite = CU_add_suite("test_builtins", NULL, NULL);
	if (NULL == pSuite)
	{
		CU_cleanup_registry();
		return (CU_get_error());
	}
	CU_add_test(pSuite, "test_cmd_echo", test_cmd_echo);
	CU_add_test(pSuite, "test_cmd_pwd", test_cmd_pwd);
    CU_add_test(pSuite, "test_cmd_env", test_cmd_env);
    CU_add_test(pSuite, "test_cmd_unset", test_cmd_unset);
	CU_add_test(pSuite, "test_cmd_export", test_cmd_export);
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();
	return (CU_get_error());
}
