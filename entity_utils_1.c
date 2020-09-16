/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity_utils_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/16 17:06:43 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/16 18:23:00 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libecs.h"

uint64_t		get_component_list_index(t_world *world, uint64_t component_id)
{
	void		*get_res;

	get_res = hash_map_get(world->component_to_list, component_id);
	return (get_res == 0 ? 0 : *(uint64_t*)&get_res);
}

uint64_t		component_mask(uint64_t components, uint64_t shift)
{
	return ((1ULL << shift) & components);
}

t_bool			parse_components(t_world *world, uint64_t components,
				va_list variables, uint64_t entity_index)
{
	t_component	val;
	uint64_t	component_index;
	void		*component_data;
	int			shift;

	shift = ECS_MAX_COMPONENTS - 1;
	while (components != 0)
	{
		if (component_mask(components, shift) != 0)
		{
			val = (t_component)va_arg(variables, t_component);
			if (!hash_map_has_key(world->component_to_list,
				component_mask(components, shift)))
				world_component_add(world, component_mask(components, shift));
			component_index = get_component_list_index(world,
				component_mask(components, shift));
			if (!(component_data = malloc(sizeof(char) * val.size)) &&
				ft_dprintf(2, "Failed to malloc component for entity\n"))
				return (false);
			ft_printf("Adding size: %d to component_index: %d data: %p\n",
				val.size, component_index, component_data);
			ft_memcpy(component_data, val.data, val.size);
			hash_map_add(world->component_list[component_index],
				entity_index, component_data);
			components >>= 1;
		}
		shift--;
	}
	return (true);
}

t_bool			world_entity_valid(t_world *world,
				uint64_t entity_index)
{
	return (world->entities[entity_index] > ECS_EMPTY_ENTITY);
}