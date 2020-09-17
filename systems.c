/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   systems.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/15 23:07:01 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/17 23:23:30 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libecs.h"

void			ecs_world_system_add(t_ecs_world *world, t_system system)
{
	uint64_t			next_free_index;

	if (system.system_id == ECS_SYSTEM_EMPTY)
		ft_dprintf(2, "Can't add system with id ECS_SYSTEM_EMPTY (%D)",
			ECS_SYSTEM_EMPTY);
	ft_memcpy(&world->systems[world->next_free_system_index],
		&system, sizeof(t_system));
	hash_map_add(world->system_to_list, system.system_id,
		(void*)world->next_free_system_index);
	next_free_index = world->next_free_system_index + 1;
	while (next_free_index < world->num_systems &&
		world->systems[next_free_index].system_id != ECS_SYSTEM_EMPTY)
		next_free_index++;
	world->next_free_system_index = next_free_index;
	world->num_systems++;
}

void			ecs_world_system_remove(t_ecs_world *world, uint64_t system_id)
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
			hash_map_delete(world->system_to_list, system_id);
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

uint64_t		ecs_system_index(t_ecs_world *world, uint64_t system_id)
{
	void		*get_res;

	get_res = hash_map_get(world->system_to_list, system_id);
	return (get_res == 0 ? 0 : *(uint64_t*)&get_res);
}

void			ecs_systems_run(t_ecs_world *world)
{
	uint64_t	i;
	uint64_t	entity_index;
	uint64_t	removed_systems;

	if (world->num_entities == 0)
		return ;
	entity_index = -1;
	while (++entity_index < world->max_entities)
	{
		if (world->entities[entity_index] == 0)
			continue ;
		i = -1;
		removed_systems = 0;
		while (++i < world->num_systems + removed_systems)
		{
			if (world->systems[i].system_id != ECS_SYSTEM_EMPTY)
				world->systems[i].system_handle_func(world,
					world->entities[entity_index]);
			else if (world->systems[i].system_id == ECS_SYSTEM_EMPTY)
				removed_systems++;
		}
	}
}

void			ecs_system_update_params(t_ecs_world *world,
				uint64_t system_id, void *params, size_t params_size)
{
	uint64_t	system_index;

	if (!hash_map_has_key(world->system_to_list, system_id))
		return ;
	system_index = ecs_system_index(world, system_id);
	ft_memcpy(&world->systems[system_index].params, params, params_size);
}
