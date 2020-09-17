/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   system_tests.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/15 23:15:19 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/17 22:57:03 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "oh_test.h"
#include "libecs.h"

static void		mock_system_handle(t_ecs_world *world, uint64_t entity)
{
	(void)entity;
	(void)world;
}

const char		*test_world_system_add(void)
{
	t_system	system;
	t_ecs_world		*world;

	system.system_id = 5;
	system.system_handle_func = mock_system_handle;
	system.components_mask = ECS_NO_COMPONENT;
	system.params = NULL;
	world = ecs_world_create("Test world", 64);
	ecs_world_system_add(world, system);
	oh_assert("World system adds wrong 1", world->num_systems == 1 &&
		world->systems[0].system_id == 5 &&
		world->next_free_system_index == 1);
	system.system_id = 16;
	ecs_world_system_add(world, system);
	oh_assert("World system adds wrong 2", world->num_systems == 2 &&
		world->systems[1].system_id == 16 &&
		world->next_free_system_index == 2);
	ecs_world_destroy(world);
	return (0);
}

const char		*test_world_system_remove(void)
{
	t_system	system;
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
	oh_assert("World system removes wrong 1", world->num_systems == 1 &&
		world->systems[0].system_id == ECS_SYSTEM_EMPTY &&
		world->next_free_system_index == 0);
	ecs_world_system_remove(world, 111);
	ecs_world_system_remove(world, 16);
	oh_assert("World system removes wrong 2", world->num_systems == 0 &&
		world->systems[0].system_id == ECS_SYSTEM_EMPTY &&
		world->systems[1].system_id == ECS_SYSTEM_EMPTY &&
		world->next_free_system_index == 0);
	ecs_world_destroy(world);
	return (0);
}
