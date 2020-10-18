/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   systems_parallel.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 00:59:45 by ohakola           #+#    #+#             */
/*   Updated: 2020/10/18 22:04:07 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libecs_internals.h"

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
			if (entity_matches_system(*data->world, data->systems, e_i, i))
				world->systems[i].system_handle_func(world, e_i);
			else if (world->systems[i].system_id == ECS_SYSTEM_EMPTY)
				removed_systems++;
		}
	}
}

static void		run_system_on_entity(t_system_parallel *data,
					int32_t e_i, uint64_t system_i, uint64_t *removed_systems)
{
	t_ecs_world			*world;

	world = *data->world;
	if (entity_matches_system(*data->world, data->systems,
		data->entity_group->ids[e_i], system_i))
		world->systems[system_i].system_handle_func(world,
			data->entity_group->ids[e_i]);
	else if (world->systems[system_i].system_id == ECS_SYSTEM_EMPTY)
		(*removed_systems)++;
}

static void		ecs_systems_run_entities_per_thread(void *params)
{
	uint64_t			i;
	int64_t				e_i;
	uint64_t			removed_systems;
	t_ecs_world			*world;
	t_system_parallel	*d;

	d = (t_system_parallel*)params;
	world = *d->world;
	if (world->num_entities == 0 || d->num_threads == 0 ||
		d->entity_group->num_ids == 0)
		return ;
	e_i = -1;
	while (++e_i < d->entity_group->num_ids)
	{
		if (world->entities[d->entity_group->ids[e_i]] == 0)
			continue ;
		i = -1;
		removed_systems = 0;
		while (++i < world->num_systems + removed_systems)
			run_system_on_entity(d, e_i, i, &removed_systems);
	}
}

/*
** Do not call this on systems that remove or add entities. The central
** entity data in world should not change, because doing that in parallel
** will cause unexpected bugs. Only run this on component updates.
*/

void			ecs_systems_run_parallel(t_thread_pool *tp,
					t_ecs_world *world, uint64_t systems)
{
	int32_t					i;
	t_system_parallel		data[tp->num_threads];
	uint64_t				max_num_entities;

	i = -1;
	max_num_entities = world->num_entities + (world->next_vacancy_index + 1);
	while (++i < (int32_t)tp->num_threads)
	{
		data[i].num_threads = tp->num_threads;
		data[i].systems = systems;
		data[i].thread_id = i;
		data[i].world = &world;
		data[i].min_entity_index = i * (max_num_entities / tp->num_threads);
		data[i].max_entity_index = (i + 1) *
			(max_num_entities / tp->num_threads);
		if (i == (int32_t)tp->num_threads - 1)
			data[i].max_entity_index = max_num_entities;
		if (!thread_pool_add_work(tp,
			(void*)ecs_systems_run_per_thread, &data[i]))
			ft_dprintf(2, "Failed to add work to thread pool.\n");
	}
	thread_pool_wait(tp);
}

void			ecs_systems_run_parallel_on_entities(t_thread_pool *tp,
					t_ecs_world *world, uint64_t systems,
					t_hash_table *entities_by_thread)
{
	int32_t					i;
	t_system_parallel		data[tp->num_threads];

	i = -1;
	while (++i < (int32_t)tp->num_threads)
	{
		data[i].num_threads = tp->num_threads;
		data[i].systems = systems;
		data[i].thread_id = i;
		data[i].world = &world;
		data[i].entity_group =
			(t_entity_id_group*)hash_map_get(entities_by_thread, i);
		if (data[i].entity_group == NULL)
			ft_dprintf(2, "Invalid entities by thread input\n");
		if (!thread_pool_add_work(tp,
			(void*)ecs_systems_run_entities_per_thread, &data[i]))
			ft_dprintf(2, "Failed to add work to thread pool.\n");
	}
	thread_pool_wait(tp);
}
