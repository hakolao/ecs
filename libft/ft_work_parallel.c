/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_work_parallel.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/21 14:25:55 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/21 14:29:50 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** This function performs parallel work (worker_f)
** After the work, threads are joined back.
*/

void						work_parallel(int num_threads, void *params,
							void (*worker_f)(void *params))
{
	pthread_t				threads[num_threads];
	int						i;

	i = -1;
	while (++i < num_threads)
		if (pthread_create(&threads[i], NULL, (void*)worker_f, params))
			ft_dprintf(2, "Failed to create thread.\n");
	i = 0;
	while (i < num_threads)
		pthread_join(threads[i++], NULL);
}
