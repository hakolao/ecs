/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entities_tests.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/15 23:41:13 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/30 01:16:57 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "oh_test.h"
#include "libecs.h"
#include "test_helpers.h"
#include "libecs_internals.h"

static uint64_t		add_mock_entity(t_ecs_world *world)
{
	return (ecs_world_entity_add(world, 2,
		&(t_component){.id = comp_position, .size = sizeof(t_position),
		.data = &(t_position){.x = 5, .y = 10}},
		&(t_component){.id = comp_velocity, .size = sizeof(t_velocity),
		.data = &(t_velocity){.x = 1.0, .y = 2.0}}));
}

const char			*test_world_entity_add(void)
{
	t_ecs_world		*world;
	void			*entity_comp;
	int64_t			entity_index;

	world = ecs_world_create("Test", 128);
	entity_index = add_mock_entity(world);
	entity_comp = ecs_world_entity_component_get(world, entity_index,
		comp_position);
	OH_ASSERT("World entity adding fails 1", world->num_components == 2 &&
		world->num_entities == 1 && entity_index == 0 &&
		world->next_free_entity_index == 1 &&
		((t_position*)entity_comp)->x == 5 &&
		((t_position*)entity_comp)->y == 10);
	entity_index = add_mock_entity(world);
	entity_comp =
		ecs_world_entity_component_get(world, entity_index, comp_velocity);
	OH_ASSERT("World entity adding fails 2", world->num_components == 2 &&
		world->num_entities == 2 && entity_index == 1 &&
		world->next_free_entity_index == 2 &&
		((t_velocity*)entity_comp)->x == 1.0 &&
		((t_velocity*)entity_comp)->y == 2.0);
	ecs_world_destroy(world);
	return (0);
}

/*
** entities: 1, 2, 3, 0, 0, 0...
** remove index 1 (2):
** entities: 1, 0, 3, 0, 0, 0...
** vacant_entities: 1, 0, 0, 0...
** next_vacancy_index: 0
** add entity from vacancy array (at index 0)
** entities: 1, new, 3, 0, 0, 0...
** vacant_entities: 0, 0, 0...
*/

const char			*test_world_entity_remove(void)
{
	t_ecs_world		*world;
	int64_t			entity_index1;
	int64_t			entity_index2;
	int64_t			entity_index3;

	world = ecs_world_create("Test", 128);
	entity_index1 = add_mock_entity(world);
	entity_index2 = add_mock_entity(world);
	entity_index3 = add_mock_entity(world);
	ecs_world_entity_remove(world, entity_index2);
	OH_ASSERT("World entity removal fails1", world->num_entities == 2 &&
		world->next_free_entity_index == 3 && world->next_vacancy_index == 0 &&
		world->vacant_entities[world->next_vacancy_index] == 1);
	entity_index2 = add_mock_entity(world);
	OH_ASSERT("World entity add after removal fails",
		world->num_entities == 3 && world->next_free_entity_index == 3 &&
		world->next_vacancy_index == -1 &&
		world->vacant_entities[0] == 0);
	ecs_world_entity_remove(world, entity_index1);
	ecs_world_entity_remove(world, entity_index2);
	ecs_world_entity_remove(world, entity_index3);
	OH_ASSERT("World entity remove all fails", world->num_entities == 0 &&
		world->next_free_entity_index == 3 && world->next_vacancy_index == 2);
	ecs_world_destroy(world);
	return (0);
}

const char			*test_world_entity_component_add(void)
{
	t_ecs_world		*world;
	void			*ec1;
	void			*ec2;
	int64_t			index;

	world = ecs_world_create("Test", 128);
	index = add_mock_entity(world);
	ecs_world_entity_components_add(world, index, 2, &(t_component){.id =
		comp_color, .size = sizeof(t_color), .data = &(t_color){.val = 111}},
		&(t_component){.id = comp_foo, .size = sizeof(t_foo),
		.data = &(t_foo){.bar = 10.00}});
	ec1 = ecs_world_entity_component_get(world, index, comp_color);
	ec2 = ecs_world_entity_component_get(world, index, comp_foo);
	OH_ASSERT("Entity component add fail", world->num_components == 4 &&
		((t_color*)ec1)->val == 111 && ((t_foo*)ec2)->bar == 10.00);
	ecs_world_entity_components_add(world, index, 2, &(t_component){.id =
		comp_color, .size = sizeof(t_color), .data = &(t_color){.val = 12345}},
		&(t_component){.id = comp_foo, .size = sizeof(t_foo),
		.data = &(t_foo){.bar = 12.00}});
	ec1 = ecs_world_entity_component_get(world, index, comp_color);
	ec2 = ecs_world_entity_component_get(world, index, comp_foo);
	OH_ASSERT("Entity component add fail", world->num_components == 4 &&
		((t_color*)ec1)->val == 12345 && ((t_foo*)ec2)->bar == 12.00);
	ecs_world_destroy(world);
	return (0);
}

const char			*test_world_entity_utils(void)
{
	t_ecs_world		*world;
	int64_t			entity_index;

	world = ecs_world_create("Test", 128);
	entity_index = add_mock_entity(world);
	ecs_world_entity_components_add(world, entity_index, 2, &(t_component){.id =
		comp_color, .size = sizeof(t_color), .data = &(t_color){.val = 111}},
		&(t_component){.id = comp_foo, .size = sizeof(t_foo),
		.data = &(t_foo){.bar = 10.00}});
	ecs_world_entity_components_remove(world, entity_index, comp_color);
	OH_ASSERT("World entity component removal failed 1",
		world->entities[entity_index] ==
		(comp_foo | comp_position | comp_velocity) && world->num_entities == 1);
	ecs_world_entity_components_remove(world, entity_index, comp_foo);
	OH_ASSERT("World entity component removal failed 2",
		world->entities[entity_index] == (comp_position | comp_velocity) &&
		world->num_entities == 1);
	ecs_world_entity_components_remove(world, entity_index,
		comp_velocity | comp_position);
	OH_ASSERT("World entity component removal failed 3",
		world->entities[entity_index] == 0 &&
		world->num_entities == 0);
	ecs_world_destroy(world);
	return (0);
}
