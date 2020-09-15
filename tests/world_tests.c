/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/15 22:55:29 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/15 23:06:44 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "oh_test.h"
#include "libecs.h"

const char			*test_world_create(void)
{
	t_world	*world;

	world = world_create("Foo", 128);
	oh_assert("World name isn't correct", ft_strequ(world->name, "Foo"));
	oh_assert("World max ent isn't correct", world->max_entities == 128);
	oh_assert("World initialized correctly", world->num_components == 0 &&
		world->num_entities == 0 && world->num_systems == 0 &&
		world->next_free_component_index == 0 &&
		world->next_free_entity_index == 0 &&
		world->next_free_system_index == 0 &&
		world->current_entity_index == -1);
	world_destroy(world);
	return (0);
}
