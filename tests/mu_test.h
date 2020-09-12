/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mu_test.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/16 19:15:20 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/12 15:29:56 by ohakola          ###   ########.fr       */
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
** using vector and a test_state struct.
*/

#define oh_assert(message, test) \
	do { \
		if (!(test)) return message; \
	} while (0)
#define oh_run_test(test) \
	do { \
		const char *message = test(); \
		tests_run++; \
		t_test_state state = \
			test_state_create(message ? false : true, \
				message, tests_run - 1); \
		test_states[tests_run - 1] = state; \
		ft_printf(" test: %d: %s\n", state.id, \
			state.success ? "Success" : state.result); \
	} while (0)

extern int tests_run;
extern t_test_state test_states[MAX_TESTS];

#endif
