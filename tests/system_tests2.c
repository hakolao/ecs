/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   system_tests2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/01 17:45:30 by ohakola           #+#    #+#             */
/*   Updated: 2020/10/13 13:05:19 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "oh_test.h"
#include "libecs.h"
#include "test_helpers.h"

static void		mock_system_handle(t_ecs_world *world, uint64_t entity_index)
{
	t_position		*position;

	position = (t_position*)ecs_world_entity_component_get(world,
		entity_index, comp_position);
	position->x = 15;
	position->y = 28;
}

static char		*test_multiple(t_ecs_world *world, uint64_t num_entities)
{
	uint64_t		e_i;
	t_position		*position;

	e_i = -1;
	while (++e_i < num_entities)
	{
		position = (t_position*)ecs_world_entity_component_get(world, e_i,
			comp_position);
		OH_ASSERT("Systems run multiple on many entities\n",
			position->x = 15 && position->y == 28);
	}
	ecs_world_destroy(world);
	return (0);
}

const char		*test_world_system_run_many_entities(void)
{
	t_system		system;
	t_ecs_world		*world;
	uint64_t		e_i;
	uint64_t		num_entities;

	system.system_id = 5;
	system.system_handle_func = mock_system_handle;
	system.components_mask = comp_position;
	system.params = (void*)&system.system_id;
	num_entities = 128;
	world = ecs_world_create("Test world", num_entities);
	ecs_world_system_add(world, system);
	e_i = -1;
	while (++e_i < num_entities)
		ecs_world_entity_add(world, 1, &(t_component){.id = comp_position,
			.size = sizeof(t_position), .data = &(t_position){.x = 5, .y = 2}});
	ecs_systems_run(world, system.system_id);
	return (test_multiple(world, num_entities));
}

const char		*test_world_system_run_many_per_thread(void)
{
	t_system		system;
	t_ecs_world		*world;
	uint64_t		e_i;
	uint64_t		num_entities;
	t_hash_table	*ents_map;

	ents_map = hash_map_create(2);
	system.system_id = 5;
	system.system_handle_func = mock_system_handle;
	system.components_mask = comp_position;
	system.params = (void*)&system.system_id;
	num_entities = 10;
	world = ecs_world_create("Test world", num_entities);
	ecs_world_system_add(world, system);
	e_i = -1;
	while (++e_i < num_entities)
		ecs_world_entity_add(world, 1, &(t_component){.id = comp_position,
			.size = sizeof(t_position), .data = &(t_position){.x = 5, .y = 2}});
	hash_map_add(ents_map, 0, &(t_entity_id_group){
		.ids = (uint64_t[5]){0, 1, 2, 3, 4}, .num_ids = 5});
	hash_map_add(ents_map, 1, &(t_entity_id_group){
		.ids = (uint64_t[5]){5, 6, 7, 8, 9}, .num_ids = 5});
	ecs_systems_run_parallel_on_entities(2, world, system.system_id, ents_map);
	hash_map_destroy(ents_map);
	return (test_multiple(world, num_entities));
}
