/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entities_tests.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/15 23:41:13 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/16 18:25:53 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "oh_test.h"
#include "libecs.h"
#include "test_helpers.h"
#include "libecs_internal_entity_utils.h"

const char		*test_world_entity_add(void)
{
	t_world		*world;
	t_component	comp1;
	t_component	comp2;
	void		*entity_comp;
	int64_t		entity_index;

	world = world_create("Test", 128);
	comp1.data = malloc(sizeof(t_position));
	comp1.size = sizeof(t_position);
	((t_position*)comp1.data)->x = 5;
	((t_position*)comp1.data)->y = 10;
	comp2.data = malloc(sizeof(t_velocity));
	comp2.size = sizeof(t_velocity);
	((t_velocity*)comp1.data)->x = 2.5;
	((t_velocity*)comp1.data)->y = 1.9;
	entity_index = world_entity_add(world,
		component_velocity | component_position, comp1, comp2);
	entity_comp =
		world_entity_component_get(world, entity_index, component_position);
	for (size_t i = 0; i < comp1.size; i++)
	{
		ft_putchar(((char*)entity_comp)[i]);
	}
	
	ft_printf("%d %d\n", ((t_position*)entity_comp)->x, comp1.size);
	oh_assert("World component adding fails 1", world->num_components == 2 &&
		world->num_entities == 1 && entity_index == 0 &&
		world->next_free_entity_index == 1 &&
		((t_position*)entity_comp)->x == 5 &&
		((t_position*)entity_comp)->y == 10);
	entity_index = world_entity_add(world,
		component_velocity | component_position, comp1, comp2);
	entity_comp =
		world_entity_component_get(world, entity_index, component_velocity);
	oh_assert("World component adding fails 2", world->num_components == 2 &&
		world->num_entities == 2 && entity_index == 1 &&
		world->next_free_entity_index == 2 &&
		((t_velocity*)entity_comp)->x == 2.5 &&
		((t_velocity*)entity_comp)->y == 1.9);
	free(comp1.data);
	free(comp2.data);
	world_destroy(world);
	return (0);
}
