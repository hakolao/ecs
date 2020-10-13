/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/16 19:23:49 by ohakola           #+#    #+#             */
/*   Updated: 2020/10/13 12:57:58 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "oh_test.h"
#include "tests.h"

int g_tests_run = 0;
t_test_state g_test_states[MAX_TESTS];

static void		all_tests(void)
{
	ft_printf("World tests:\n");
	OH_RUN_TEST(test_world_create);
	ft_printf("World system tests:\n");
	OH_RUN_TEST(test_world_system_add);
	OH_RUN_TEST(test_world_system_remove);
	OH_RUN_TEST(test_world_system_run);
	OH_RUN_TEST(test_world_system_run_many);
	OH_RUN_TEST(test_world_system_run_many_entities);
	OH_RUN_TEST(test_world_system_run_many_per_thread);
	ft_printf("World component tests:\n");
	OH_RUN_TEST(test_world_component_add);
	OH_RUN_TEST(test_world_component_remove);
	ft_printf("World entity tests:\n");
	OH_RUN_TEST(test_world_entity_add);
	OH_RUN_TEST(test_world_entity_remove);
	OH_RUN_TEST(test_world_entity_component_add);
	OH_RUN_TEST(test_world_entity_utils);
}

int				main(void)
{
	t_bool				success;
	int					failures;
	int					i;

	all_tests();
	success = true;
	failures = 0;
	i = -1;
	while (++i < g_tests_run)
	{
		if (!g_test_states[i].success)
		{
			success = false;
			failures++;
		}
	}
	ft_printf("===========\n");
	if (!success)
		ft_printf("%d out of %d TESTS FAILED\n", failures, g_tests_run);
	else
		ft_printf("ALL %d TESTS PASSED\n", g_tests_run);
	return (!success);
}
