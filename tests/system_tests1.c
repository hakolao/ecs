/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   system_tests1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/15 23:15:19 by ohakola           #+#    #+#             */
/*   Updated: 2020/10/18 22:02:51 by ohakola          ###   ########.fr       */
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

const char		*test_world_system_add(void)
{
	t_system		system;
	t_ecs_world		*world;

	system.system_id = 5;
	system.system_handle_func = mock_system_handle;
	system.components_mask = ECS_NO_COMPONENT;
	system.params = NULL;
	world = ecs_world_create("Test world", 64);
	ecs_world_system_add(world, system);
	OH_ASSERT("World system adds wrong 1", world->num_systems == 1 &&
		world->systems[0].system_id == 5 &&
		world->next_free_system_index == 1);
	system.system_id = 16;
	ecs_world_system_add(world, system);
	OH_ASSERT("World system adds wrong 2", world->num_systems == 2 &&
		world->systems[1].system_id == 16 &&
		world->next_free_system_index == 2);
	ecs_world_destroy(world);
	return (0);
}

const char		*test_world_system_remove(void)
{
	t_system		system;
	t_ecs_world		*world;

	system.system_id = 5;
	system.system_handle_func = mock_system_handle;
	system.components_mask = ECS_NO_COMPONENT;
	system.params = NULL;
	world = ecs_world_create("Test world", 64);
	ecs_world_system_add(world, system);
	system.system_id = 16;
	ecs_world_system_add(world, system);
	ecs_world_system_remove(world, 5);
	OH_ASSERT("World system removes wrong 1", world->num_systems == 1 &&
		world->systems[0].system_id == ECS_SYSTEM_EMPTY &&
		world->next_free_system_index == 0);
	ecs_world_system_remove(world, 111);
	ecs_world_system_remove(world, 16);
	OH_ASSERT("World system removes wrong 2", world->num_systems == 0 &&
		world->systems[0].system_id == ECS_SYSTEM_EMPTY &&
		world->systems[1].system_id == ECS_SYSTEM_EMPTY &&
		world->next_free_system_index == 0);
	ecs_world_destroy(world);
	return (0);
}

const char		*test_world_system_run(void)
{
	t_system		system;
	t_ecs_world		*world;
	t_position		*position;

	system.system_id = 5;
	system.system_handle_func = mock_system_handle;
	system.components_mask = comp_position;
	system.params = (void*)&system.system_id;
	world = ecs_world_create("Test world", 64);
	ecs_world_system_add(world, system);
	ecs_world_entity_add(world, 1, &(t_component){.id = comp_position, .size =
		sizeof(t_position), .data = &(t_position){.x = 5, .y = 10}});
	ecs_systems_run_single(world, system.system_id);
	position = (t_position*)ecs_world_entity_component_get(world, 0,
		comp_position);
	OH_ASSERT("System run single did not work right\n",
		position->x = 15 && position->y == 28);
	ecs_world_destroy(world);
	return (0);
}

const char		*test_world_system_run_many(void)
{
	t_system		system;
	t_ecs_world		*world;
	t_position		*pos;
	t_thread_pool	*tp;

	tp = thread_pool_create(2);
	system.system_id = 5;
	system.system_handle_func = mock_system_handle;
	system.components_mask = comp_position;
	system.params = (void*)&system.system_id;
	world = ecs_world_create("Test world", 64);
	ecs_world_system_add(world, system);
	ecs_world_entity_add(world, 1, &(t_component){.id = comp_position, .size =
		sizeof(t_position), .data = &(t_position){.x = 5, .y = 10}});
	ecs_systems_run(world, system.system_id);
	pos = (t_position*)ecs_world_entity_component_get(world, 0,
		comp_position);
	OH_ASSERT("Systems run did not work right\n", pos->x = 15 && pos->y == 28);
	ecs_world_entity_add(world, 1, &(t_component){.id = comp_position, .size =
		sizeof(t_position), .data = &(t_position){.x = 5, .y = 10}});
	ecs_systems_run_parallel(tp, world, system.system_id);
	pos = (t_position*)ecs_world_entity_component_get(world, 1,
		comp_position);
	OH_ASSERT("Systems run parallel did not work right\n",
		pos->x = 15 && pos->y == 28);
	ecs_world_destroy(world);
	thread_pool_destroy(tp);
	return (0);
}
