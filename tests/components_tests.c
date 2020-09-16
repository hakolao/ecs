/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   components_tests.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/15 23:41:10 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/16 18:13:47 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "oh_test.h"
#include "libecs.h"

const char		*test_world_component_add(void)
{
	t_world		*world;
	uint64_t	component1;
	uint64_t	component2;
	void		*get_res;

	world = world_create("Test world", 64);
	component1 = 1 << 2;
	component2 = 1 << 3;
	world_component_add(world, component1);
	get_res = hash_map_get(world->component_to_list, component1);
	oh_assert("Adding component to world failed", get_res == NULL &&
		hash_map_has_key(world->component_to_list, component1) &&
		world->num_components == 1 && world->next_free_component_index == 1);
	world_component_add(world, component1);
	oh_assert("Adding same component to world did not fail as it should",
		get_res == NULL &&
		hash_map_has_key(world->component_to_list, component1) &&
		world->num_components == 1 && world->next_free_component_index == 1);
	world_component_add(world, component2);
	get_res = hash_map_get(world->component_to_list, component2);
	oh_assert("Adding new component to world failed", get_res != NULL &&
		world->num_components == 2 && world->next_free_component_index == 2 &&
		*(uint64_t*)&get_res == 1);
	world_destroy(world);
	return (0);
}

const char		*test_world_component_remove(void)
{
	t_world		*world;
	uint64_t	component1;
	uint64_t	component2;
	void		*get_res;

	world = world_create("Test world", 64);
	component1 = 1 << 2;
	component2 = 1 << 3;
	world_component_add(world, component1);
	world_component_add(world, component2);
	oh_assert("Precheck in component removal failed",
		world->num_components == 2 && world->next_free_component_index == 2);
	world_component_remove(world, component1);
	get_res = hash_map_get(world->component_to_list, component1);
	oh_assert("Removing component failed 1",
		world->num_components == 1 && world->next_free_component_index == 0 &&
		get_res == NULL);
	world_component_remove(world, component2);
	get_res = hash_map_get(world->component_to_list, component2);
	oh_assert("Removing component failed 2",
		world->num_components == 0 && world->next_free_component_index == 0 &&
		get_res == NULL);
	world_destroy(world);
	return (0);
}