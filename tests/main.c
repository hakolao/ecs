/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/16 19:23:49 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/15 21:16:04 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "oh_test.h"
#include "tests.h"

int tests_run = 0;
t_test_state test_states[MAX_TESTS];

static void all_tests() {
	oh_run_test(test_hash_map_create);
	oh_run_test(test_hash_map_add);
	oh_run_test(test_hash_map_get);
}

int main(void) {
	t_bool				success;
	int					failures;
	int					i;

	all_tests();
	success = true;
	failures = 0;
	i = -1;
	while (++i < tests_run)
	{
		if (!test_states[i].success)
		{
			success = false;
			failures++;
		}
	}
	ft_printf("===========\n");
	if (!success) {
		ft_printf("%d out of %d TESTS FAILED\n", failures, tests_run);
	} else {
		ft_printf("ALL %d TESTS PASSED\n", tests_run);
	}
	return !success;
}
