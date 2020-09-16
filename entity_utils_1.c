/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity_utils_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/16 17:06:43 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/16 17:15:05 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libecs.h"

size_t			get_component_size(t_world *world,
				uint64_t component_list_index)
{
	void		*get_size;
	size_t		malloc_size;

	get_size = hash_map_get(world->component_list[component_list_index],
		ECS_COMPONENT_SIZE_KEY);
	malloc_size = get_size == 0 ? 0 : *(size_t*)&get_size;
	return (malloc_size);
}

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
	t_component	*val;
	size_t		malloc_size;
	uint64_t	component_index;
	void		*component_data;
	int			shift;

	shift = ECS_MAX_COMPONENTS - 1;
	while (components != 0)
	{
		if (component_mask(components, shift) != 0)
		{
			val = (t_component*)va_arg(variables, t_component *);
			if (!hash_map_has_key(world->component_to_list,
				component_mask(components, shift)))
				world_component_add(world,
					component_mask(components, shift), val->size);
			component_index = get_component_list_index(world,
				component_mask(components, shift));
			malloc_size = get_component_size(world, component_index);
			if (!(component_data = malloc(malloc_size)) &&
				ft_dprintf(2, "Failed to malloc size for component when adding\n"))
				return (false);
			ft_memcpy(component_data, val->data, malloc_size);
			hash_map_add(world->component_list[component_index],
				entity_index, component_data);
		}
		components >>= 1;
		shift--;
	}
	return (true);
}

t_bool			world_entity_valid(t_world *world,
				uint64_t entity_index)
{
	return (world->entities[entity_index] > ECS_EMPTY_ENTITY);
}