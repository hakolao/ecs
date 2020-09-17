/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entities_tests.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/15 23:41:13 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/17 13:58:44 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "oh_test.h"
#include "libecs.h"
#include "test_helpers.h"
#include "libecs_internal_entity_utils.h"

static uint64_t		add_mock_entity(t_world *world)
{
	return (world_entity_add(world, 2,
		&(t_component){.id = comp_position, .size = sizeof(t_position),
		.data = &(t_position){.x = 5, .y = 10}},
		&(t_component){.id = comp_velocity, .size = sizeof(t_velocity),
		.data = &(t_velocity){.x = 1.0, .y = 2.0}}));
}

const char			*test_world_entity_add(void)
{
	t_world		*world;
	void		*entity_comp;
	int64_t		entity_index;

	world = world_create("Test", 128);
	entity_index = add_mock_entity(world);
	entity_comp = world_entity_component_get(world, entity_index,
		comp_position);
	oh_assert("World component adding fails 1", world->num_components == 2 &&
		world->num_entities == 1 && entity_index == 0 &&
		world->next_free_entity_index == 1 &&
		((t_position*)entity_comp)->x == 5 &&
		((t_position*)entity_comp)->y == 10);
	entity_index = add_mock_entity(world);
	entity_comp =
		world_entity_component_get(world, entity_index, comp_velocity);
	oh_assert("World component adding fails 2", world->num_components == 2 &&
		world->num_entities == 2 && entity_index == 1 &&
		world->next_free_entity_index == 2 &&
		((t_velocity*)entity_comp)->x == 1.0 &&
		((t_velocity*)entity_comp)->y == 2.0);
	world_destroy(world);
	return (0);
}
