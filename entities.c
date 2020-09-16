/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entities.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/15 23:41:23 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/16 16:37:48 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libecs.h"

static size_t	get_component_size(t_world *world,
				uint64_t component_list_index)
{
	void		*get_size;
	size_t		malloc_size;

	get_size = hash_map_get(world->component_list[component_list_index],
		ECS_COMPONENT_SIZE_KEY);
	malloc_size = get_size == 0 ? 0 : *(size_t*)&get_size;
	return (malloc_size);
}

static uint64_t	get_component_list_index(t_world *world, uint64_t component_id)
{
	void		*get_res;

	get_res = hash_map_get(world->component_to_list, component_id);
	return (get_res == 0 ? 0 : *(uint64_t*)&get_res);
}

static t_bool	parse_components(t_world *world, uint64_t components,
				va_list variables, int shift)
{
	t_component	*val;
	uint64_t	component_id;
	size_t		malloc_size;
	uint64_t	component_list_index;
	void		*component_data;

	while (components != 0)
	{
		component_id = (1ULL << shift) & components;
		if (component_id != 0)
		{
			val = (t_component*)va_arg(variables, t_component *);
			if (!hash_map_has_key(world->component_to_list, component_id))
				world_component_add(world, component_id, val->size);
			component_list_index = get_component_list_index(world, component_id);
			malloc_size = get_component_size(world, component_list_index);
			if (!(component_data = malloc(malloc_size)) &&
				ft_dprintf(2, "Failed to malloc size for component when adding\n"))
				return (false);
			ft_memcpy(component_data, val->data, malloc_size);
			hash_map_add(world->component_list[component_list_index],
				world->next_free_entity_index, component_data);
		}
		components >>= 1;
		shift--;
	}
	return (true);
}

uint64_t		world_entity_add(t_world *world, uint64_t components, ...)
{
	va_list		variables;
	uint64_t	entity_mask;
	t_bool		res;
	
	va_start(variables, components);
	entity_mask = components;
	res = parse_components(world, components, variables,
		ECS_MAX_COMPONENTS - 1);
	va_end(variables);
	if (res == false)
		return (-1);
	else
		res = world->next_free_entity_index;
	world->entities[world->next_free_entity_index] = entity_mask;
	if (world->next_vacant_entity_index != -1)
		world->next_free_entity_index =
			world->vacant_entities[world->next_vacant_entity_index--];
	else
		world->next_free_entity_index++;
	return (res);
}

uint64_t		world_entity_get(t_world *world, uint64_t entity)
{
	return (world->entities[entity]);
}


void			world_entity_remove(t_world *world, uint64_t entity_index)
{
	uint64_t		entity_components;
	uint64_t		component_id;
	uint64_t		component_list_index;
	int				shift;

	if (entity_index >= world->max_entities)
		return (void)ft_dprintf(2,
			"Invalid id %d in removal\n", entity_index);
	entity_components = world_entity_get(world, entity_index);
	shift = ECS_MAX_COMPONENTS - 1;
	while (entity_components != 0)
	{
		component_id = (1ULL << shift)	& entity_components;
		if (component_id != 0)
		{
			component_list_index =
				get_component_list_index(world, component_id);
			hash_map_delete(world->component_list[component_list_index],
				entity_index);
		}
		entity_components >>= 1;
	}
	world->entities[entity_index] = 0;
	world->next_vacant_entity_index++;
	world->vacant_entities[world->next_vacant_entity_index] = entity_index;
}

// void			world_entity_component_add(t_world *world,
// 				uint64_t entity, uint64_t component)
// {

// }

// void			world_entity_component_remove(t_world *world,
// 				uint64_t entity, uint64_t component)
// {
	
// }

// t_bool			world_entity_valid(t_world *world,
// 				uint64_t entity)
// {
	
// }

// t_bool			world_entity_contains(t_world *world, uint64_t entity,
// 				uint64_t components)
// {
	
// }

// void			*world_entity_component_get(t_world *world,
// 				uint64_t entity, uint64_t component)
// {
	
// }