/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   systems.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/15 23:07:01 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/21 14:54:00 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libecs.h"

static t_bool	contains_system(uint64_t systems_group, uint64_t system)
{
	return ((system & systems_group) == system);
}

/*
** Adds a system to the world. These systems contain a handle function which
** are run on each entity.
*/

void			ecs_world_system_add(t_ecs_world *world, t_system system)
{
	uint64_t			next_free_index;

	if (system.system_id == ECS_SYSTEM_EMPTY)
		ft_dprintf(2, "Can't add system with id ECS_SYSTEM_EMPTY (%D)",
			ECS_SYSTEM_EMPTY);
	ft_memcpy(&world->systems[world->next_free_system_index],
		&system, sizeof(t_system));
	hash_map_add(world->index_by_system, system.system_id,
		(void*)world->next_free_system_index);
	next_free_index = world->next_free_system_index + 1;
	while (next_free_index < world->num_systems &&
		world->systems[next_free_index].system_id != ECS_SYSTEM_EMPTY)
		next_free_index++;
	world->next_free_system_index = next_free_index;
	world->num_systems++;
}

/*
** Removes a system from the world.
*/

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
			hash_map_delete(world->index_by_system, system_id);
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

/*
** A convenience method to run each system on all world entities.
** System ids can be given like (system_render | system_move | system_color).
** All matching systems are checked on entities that contains that system's
** components_mask.
*/

void			ecs_systems_run(t_ecs_world *world, uint64_t systems)
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
			if (contains_system(systems, world->systems[i].system_id) &&
				world->systems[i].system_id != ECS_SYSTEM_EMPTY &&
				ecs_world_entity_contains(world->entities[entity_index],
				world->systems[i].components_mask))
				world->systems[i].system_handle_func(world, entity_index);
			else if (world->systems[i].system_id == ECS_SYSTEM_EMPTY)
				removed_systems++;
		}
	}
}

/*
** A convenience method to run a single system on each world entity
*/

void			ecs_systems_run_single(t_ecs_world *world, uint64_t system_id)
{
	uint64_t	entity_index;
	t_system	system;

	if (world->num_entities == 0 ||
		!hash_map_has_key(world->index_by_system, system_id))
		return ;
	system = world->systems[ecs_system_index(world, system_id)];
	entity_index = -1;
	while (++entity_index < world->max_entities)
	{
		if (world->entities[entity_index] == 0 &&
			!ecs_world_entity_contains(world->entities[entity_index],
				system.components_mask))
			continue ;
		system.system_handle_func(world, entity_index);
	}
}

/*
** A convenience method to update system params. For example app->delta_time
** could be passed here (make sure it exists the app's life time since no
** malloc is used here nor memcopying)
*/

void			ecs_system_update_params(t_ecs_world *world,
				uint64_t system_id, void *params)
{
	uint64_t	system_index;

	if (!hash_map_has_key(world->index_by_system, system_id))
		return ;
	system_index = ecs_system_index(world, system_id);
	world->systems[system_index].params = params;
}

void			ecs_systems_run_per_thread(void *params)
{
	uint64_t			i;
	uint64_t			entity_index;
	uint64_t			removed_systems;
	t_ecs_world			*world;
	t_system_parallel	*data;

	data = (t_system_parallel*)params;
	world = *data->world;
	if (world->num_entities == 0)
		return ;
	entity_index = -1;
	while (++entity_index < world->max_entities)
	{
		if (world->entities[entity_index] == 0 ||
			entity_index % data->thread_index != 0 ||
			(entity_index == 0 && data->thread_index != 0))
			continue ;
		i = -1;
		removed_systems = 0;
		while (++i < world->num_systems + removed_systems)
		{
			if (contains_system(data->systems, world->systems[i].system_id) &&
				world->systems[i].system_id != ECS_SYSTEM_EMPTY &&
				ecs_world_entity_contains(world->entities[entity_index],
				world->systems[i].components_mask))
				world->systems[i].system_handle_func(world, entity_index);
			else if (world->systems[i].system_id == ECS_SYSTEM_EMPTY)
				removed_systems++;
		}
	}
}

void			ecs_systems_run_parallel(int32_t num_threads,
				t_ecs_world *world, uint64_t systems)
{
	pthread_t				threads[num_threads];
	int						i;

	i = 0;
	while (++i < num_threads + 1)
		if (pthread_create(&threads[i], NULL, (void*)ecs_systems_run_per_thread,
			&(t_system_parallel){.systems = systems, .thread_index = i,
				.world = &world}))
			ft_dprintf(2, "Failed to create thread.\n");
	i = -1;
	while (++i < num_threads)
		pthread_join(threads[i ], NULL);
}