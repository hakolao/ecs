/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   systems_parallel.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 00:59:45 by ohakola           #+#    #+#             */
/*   Updated: 2020/10/01 16:26:26 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libecs_internals.h"

t_bool			system_and_entity_matches(t_ecs_world *world,
				uint64_t systems, uint64_t entity_index, uint64_t system_index)
{
	return (contains_system(systems,
		world->systems[system_index].system_id) &&
		world->systems[system_index].system_id != ECS_SYSTEM_EMPTY &&
		ecs_world_entity_contains(world->entities[entity_index],
		world->systems[system_index].components_mask));
}

static void		ecs_systems_run_per_thread(void *params)
{
	uint64_t			i;
	int64_t				e_i;
	uint64_t			removed_systems;
	t_ecs_world			*world;
	t_system_parallel	*data;

	data = (t_system_parallel*)params;
	world = *data->world;
	if (world->num_entities == 0 || data->num_threads == 0)
		return ;
	e_i = (int64_t)data->min_entity_index - 1;
	while (++e_i < data->max_entity_index)
	{
		if (world->entities[e_i] == 0)
			continue ;
		i = -1;
		removed_systems = 0;
		while (++i < world->num_systems + removed_systems)
		{
			if (system_and_entity_matches(*data->world, data->systems, e_i, i))
				world->systems[i].system_handle_func(world, e_i);
			else if (world->systems[i].system_id == ECS_SYSTEM_EMPTY)
				removed_systems++;
		}
	}
}

/*
** Do not call this on systems that remove or add entities. The central
** entity data in world should not change, because doing that in parallel
** will cause unexpected bugs. Only run this on component updates.
*/

void			ecs_systems_run_parallel(int32_t num_threads,
				t_ecs_world *world, uint64_t systems)
{
	pthread_t				threads[num_threads];
	int						i;
	t_system_parallel		data[num_threads];
	uint64_t				max_num_entities;

	i = -1;
	max_num_entities = world->num_entities + (world->next_vacancy_index + 1);
	while (++i < num_threads)
	{
		data[i].num_threads = num_threads;
		data[i].systems = systems;
		data[i].thread_id = i;
		data[i].world = &world;
		data[i].min_entity_index = i * (max_num_entities / num_threads);
		data[i].max_entity_index = (i + 1) *
			(max_num_entities / num_threads);
		if (i == num_threads - 1)
			data[i].max_entity_index = max_num_entities;
		if (pthread_create(&threads[i], NULL, (void*)ecs_systems_run_per_thread,
			&data[i]))
			ft_dprintf(2, "Failed to create thread.\n");
	}
	i = -1;
	while (++i < num_threads)
		pthread_join(threads[i], NULL);
}
