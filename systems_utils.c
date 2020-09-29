/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   systems_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 00:50:03 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/30 00:59:15 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libecs.h"

t_bool			contains_system(uint64_t systems_group, uint64_t system)
{
	return ((system & systems_group) == system);
}

t_system		ecs_world_system_get(t_ecs_world *world, uint64_t system_id)
{
	return (world->systems[ecs_system_index(world, system_id)]);
}

/*
** A convenience method to get system index from index_by_system hash table
** by system id. This should only be called if index_by_system hash table
** contains system_id key. Or that you are certain key is there.
*/

uint64_t		ecs_system_index(t_ecs_world *world, uint64_t system_id)
{
	void		*get_res;

	get_res = hash_map_get(world->index_by_system, system_id);
	return (get_res == 0 ? 0 : *(uint64_t*)&get_res);
}