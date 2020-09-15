/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   systems.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/15 23:07:01 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/15 23:37:29 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libecs.h"

void			world_system_add(t_world *world, t_system system)
{
	uint64_t			next_free_index;

	if (system.system_id == ECS_SYSTEM_EMPTY)
		ft_dprintf(2, "Can't add system with id ECS_SYSTEM_EMPTY (%D)",
			ECS_SYSTEM_EMPTY);
	ft_memcpy(&world->systems[world->next_free_system_index],
		&system, sizeof(t_system));
	next_free_index = world->next_free_system_index + 1;
	while (next_free_index < world->num_systems &&
		world->systems[next_free_index].system_id != ECS_SYSTEM_EMPTY)
		next_free_index++;
	world->next_free_system_index = next_free_index;
	world->num_systems++;
}

void			world_system_remove(t_world *world, uint64_t system_id)
{
	uint64_t	i;
	uint64_t	removed;

	i = -1;
	if (system_id == ECS_SYSTEM_EMPTY)
		return ;
	removed = 0;
	while (++i < world->num_systems + removed)
	{
		if (world->systems[i].system_id == system_id)
		{
			ft_memset(&world->systems[i], 0, sizeof(t_system));
			world->systems[i].system_id = ECS_SYSTEM_EMPTY;
			world->next_free_system_index = i < world->next_free_system_index ?
				i : world->next_free_system_index;
			world->num_systems--;
		}
		else if (world->systems[i].system_id == ECS_SYSTEM_EMPTY)
			removed++;
	}
}
