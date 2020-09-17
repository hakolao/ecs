/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world_tests.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/15 22:55:29 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/17 17:03:13 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "oh_test.h"
#include "libecs.h"

const char			*test_world_create(void)
{
	t_ecs_world	*world;

	world = ecs_world_create("Foo", 128);
	oh_assert("World name isn't correct", ft_strequ(world->name, "Foo"));
	oh_assert("World max ent isn't correct", world->max_entities == 128);
	oh_assert("World comoponent to list index isn't correct",
		world->max_entities == 128 && world->component_to_list != NULL);
	oh_assert("World initialized correctly", world->num_components == 0 &&
		world->num_entities == 0 && world->num_systems == 0 &&
		world->num_components == 0 &&
		world->next_free_component_index == 0 &&
		world->next_free_entity_index == 0 &&
		world->next_free_system_index == 0 &&
		world->vacant_entities[0] == -1 &&
		world->next_vacancy_index == -1);
	ecs_world_destroy(world);
	return (0);
}
