/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity_utils_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/16 17:06:43 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/17 15:30:21 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libecs.h"

static t_bool	add_entity_component_data(t_world *world, uint64_t entity_index,
				t_component *comp, uint64_t	component_index)
{
	void		*component_data;
	void		**existing_data;

	if (!hash_map_has_key(world->component_list[component_index],
		entity_index))
	{
		if (!(component_data = malloc(comp->size)) &&
			ft_dprintf(2, "Failed to malloc component for entity\n"))
			return (false);
		ft_memmove(component_data, comp->data, comp->size);
		hash_map_add(world->component_list[component_index],
			entity_index, component_data);
	}
	else
	{
		ft_printf("Already had key for entity %d, component %d, moving data instead\n",
			entity_index, comp->id);
		component_data = hash_map_get(world->component_list[component_index],
		entity_index);
		existing_data = &component_data;
		ft_memcpy(*existing_data, comp->data, comp->size);
	}
	return (true);
}

uint64_t		get_component_list_index(t_world *world, uint64_t component_id)
{
	void		*get_res;

	get_res = hash_map_get(world->component_to_list, component_id);
	return (get_res == 0 ? 0 : *(uint64_t*)&get_res);
}

uint64_t		parse_components(t_world *world, uint64_t num_components,
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
		if (!hash_map_has_key(world->component_to_list, comp->id))
			world_component_add(world, comp->id);
		component_index = get_component_list_index(world, comp->id);
		if (!add_entity_component_data(world, entity_index, comp,
			component_index))
			return (0);
		entity_mask |= comp->id;
	}
	return (entity_mask);
}

t_bool			world_entity_valid(t_world *world,
				uint64_t entity_index)
{
	return (world->entities[entity_index] > ECS_EMPTY_ENTITY);
}
