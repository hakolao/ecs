/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/16 17:14:17 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/16 17:17:38 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libecs.h"
#include "libecs_internal_entity_utils.h"

void			entity_remove_component(t_world *world,
				uint64_t entity_index, uint64_t component_id)
{
	uint64_t		component_list_index;

	component_list_index =
		get_component_list_index(world, component_id);
	hash_map_delete(world->component_list[component_list_index],
		entity_index);
}

uint64_t		world_entity_get(t_world *world, uint64_t entity)
{
	return (world->entities[entity]);
}

t_bool			world_entity_contains(t_world *world, uint64_t entity_index,
				uint64_t components)
{
	uint64_t	entity_mask;

	if (!world_entity_valid(world, entity_index))
		return (false);
	entity_mask = world->entities[entity_index];
	return (entity_mask & components);
}