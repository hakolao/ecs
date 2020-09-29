/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity_utils_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/16 17:06:43 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/30 00:49:17 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libecs.h"

/*
** Adds entity component data through inputted t_component pointer.
** If the key exists in the component's hash map, the value gets memcopied over
** instead.
*/

static t_bool	add_entity_component_data(t_ecs_world *world,
				uint64_t entity_index, t_component *comp,
				uint64_t component_index)
{
	void		*component_data;

	if (!hash_map_has_key(world->components_by_entity[component_index],
		entity_index))
	{
		if (!(component_data = malloc(comp->size)) &&
			ft_dprintf(2, "Failed to malloc component for entity\n"))
			return (false);
		ft_memcpy(component_data, comp->data, comp->size);
		hash_map_add(world->components_by_entity[component_index],
			entity_index, component_data);
	}
	else
	{
		component_data =
			hash_map_get(world->components_by_entity[component_index],
				entity_index);
		ft_memcpy(component_data, comp->data, comp->size);
	}
	return (true);
}

/*
** Returns the index of a component_id from the world->index_by_component
** hash map. This should not be called unless the hash map has the key.
** Otherwise 0 may result unintendedly.
*/

uint64_t		ecs_component_index(t_ecs_world *world, uint64_t component_id)
{
	void		*get_res;

	get_res = hash_map_get(world->index_by_component, component_id);
	return (get_res == 0 ? 0 : *(uint64_t*)&get_res);
}

/*
** Parses components from the va_list variables by number of components.
** Given entity's value is bitwise OR'ed to contain component information.
** Index of entity is returned.
*/

uint64_t		parse_components(t_ecs_world *world, uint64_t num_components,
				va_list variables, uint64_t entity_index)
{
	t_component	*comp;
	uint64_t	component_index;
	uint64_t	entity_mask;
	int			i;

	entity_mask = 0;
	i = -1;
	while (++i < (int)num_components)
	{
		comp = (t_component*)va_arg(variables, t_component*);
		if (!hash_map_has_key(world->index_by_component, comp->id))
			ecs_world_component_add(world, comp->id);
		component_index = ecs_component_index(world, comp->id);
		if (!add_entity_component_data(world, entity_index, comp,
			component_index))
			return (0);
		entity_mask |= comp->id;
	}
	return (entity_mask);
}

/*
** If entity at given index is not empty, entity is valid.
*/

t_bool			ecs_world_entity_valid(t_ecs_world *world,
				uint64_t entity_index)
{
	return (world->entities[entity_index] > ECS_EMPTY_ENTITY);
}
