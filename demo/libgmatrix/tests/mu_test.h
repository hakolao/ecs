/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mu_test.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/16 19:15:20 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/30 01:13:18 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MU_TEST_H
# define MU_TEST_H

# include "libft.h"
# include "test_state.h"

# define MAX_TESTS 1024

/*
** Modified version of
** http://www.jera.com/techinfo/jtns/jtn002.html
** using vector and a TestState struct.
*/

#define mu_assert(message, test) \
	do { \
		if (!(test)) return message; \
	} while (0)
#define mu_run_test(test) \
	do { \
		const char *message = test(); \
		g_tests_run++; \
		t_test_state state = \
			test_state_create(message ? false : true, \
				message, g_tests_run - 1); \
		g_test_states[g_tests_run - 1] = state; \
		ft_printf(" test: %d: %s\n", state.id, \
			state.success ? "Success" : state.result); \
	} while (0)

extern int g_tests_run;
extern t_test_state g_test_states[MAX_TESTS];

#endif
