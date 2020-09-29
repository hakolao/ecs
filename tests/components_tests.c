/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   components_tests.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/15 23:41:10 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/30 01:16:57 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "oh_test.h"
#include "libecs.h"

const char		*test_world_component_add(void)
{
	t_ecs_world		*world;
	uint64_t		component1;
	uint64_t		component2;
	void			*get_res;

	world = ecs_world_create("Test world", 64);
	component1 = 1 << 2;
	component2 = 1 << 3;
	ecs_world_component_add(world, component1);
	get_res = hash_map_get(world->index_by_component, component1);
	OH_ASSERT("Adding component to world failed", get_res == NULL &&
		hash_map_has_key(world->index_by_component, component1) &&
		world->num_components == 1 && world->next_free_component_index == 1);
	ecs_world_component_add(world, component1);
	OH_ASSERT("Adding same component to world did not fail as it should",
		get_res == NULL &&
		hash_map_has_key(world->index_by_component, component1) &&
		world->num_components == 1 && world->next_free_component_index == 1);
	ecs_world_component_add(world, component2);
	get_res = hash_map_get(world->index_by_component, component2);
	OH_ASSERT("Adding new component to world failed", get_res != NULL &&
		world->num_components == 2 && world->next_free_component_index == 2 &&
		*(uint64_t*)&get_res == 1);
	ecs_world_destroy(world);
	return (0);
}

const char		*test_world_component_remove(void)
{
	t_ecs_world		*world;
	uint64_t		component1;
	uint64_t		component2;
	void			*get_res;

	world = ecs_world_create("Test world", 64);
	component1 = 1 << 2;
	component2 = 1 << 3;
	ecs_world_component_add(world, component1);
	ecs_world_component_add(world, component2);
	OH_ASSERT("Precheck in component removal failed",
		world->num_components == 2 && world->next_free_component_index == 2);
	ecs_world_component_remove(world, component1);
	get_res = hash_map_get(world->index_by_component, component1);
	OH_ASSERT("Removing component failed 1",
		world->num_components == 1 && world->next_free_component_index == 0 &&
		get_res == NULL);
	ecs_world_component_remove(world, component2);
	get_res = hash_map_get(world->index_by_component, component2);
	OH_ASSERT("Removing component failed 2",
		world->num_components == 0 && world->next_free_component_index == 0 &&
		get_res == NULL);
	ecs_world_destroy(world);
	return (0);
}
