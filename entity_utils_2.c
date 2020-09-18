/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/16 17:14:17 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/18 14:11:27 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libecs.h"
#include "libecs_internal_entity_utils.h"

/*
** Removes a component from an entity.
*/

void			entity_remove_component(t_ecs_world *world,
				uint64_t entity_index, uint64_t component_id)
{
	uint64_t		component_list_index;

	component_list_index =
		ecs_component_index(world, component_id);
	hash_map_delete(world->components_by_entity[component_list_index],
		entity_index);
}

/*
** Returns the component value of an entity (entity_mask)
*/

uint64_t		ecs_world_entity_get(t_ecs_world *world, uint64_t entity)
{
	return (world->entities[entity]);
}

/*
** Tests if an entity at index contains inputted components.
** E.g. ecs_world_entity_at_contains(world, entity_index, com1 | comp2 | comp3);
*/

t_bool			ecs_world_entity_at_contains(t_ecs_world *world,
				uint64_t entity_index, uint64_t components)
{
	uint64_t	entity_mask;

	if (!ecs_world_entity_valid(world, entity_index))
		return (false);
	entity_mask = world->entities[entity_index];
	return ((entity_mask & components) == components);
}

/*
** Tests if an entity's value contains inputted components.
*/

t_bool			ecs_world_entity_contains(uint64_t entity, uint64_t components)
{
	return ((entity & components) == components);
}
