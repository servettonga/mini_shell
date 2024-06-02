/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_cmd_echo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sehosaf <sehosaf@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 13:29:33 by sehosaf           #+#    #+#             */
/*   Updated: 2024/06/02 13:30:36 by sehosaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CUnit/Basic.h"
#include "../../src/builtin/cmd_echo.c"

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
	pSuite = CU_add_suite("cmd_echo_test_suite", 0, 0);
	if (NULL == pSuite)
	{
		CU_cleanup_registry();
		return (CU_get_error());
	}
	if ((NULL == CU_add_test(pSuite, "test_cmd_echo", test_cmd_echo)))
	{
		CU_cleanup_registry();
		return (CU_get_error());
	}
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();
	return (CU_get_error());
}
