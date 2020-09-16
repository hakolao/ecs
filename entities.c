/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entities.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/15 23:41:23 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/16 17:03:36 by ohakola          ###   ########.fr       */
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

static uint64_t	component_mask(uint64_t components, uint64_t shift)
{
	return ((1ULL << shift) & components);
}

static t_bool	parse_components(t_world *world, uint64_t components,
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

uint64_t		world_entity_add(t_world *world, uint64_t components, ...)
{
	va_list		variables;
	t_bool		res;
	
	va_start(variables, components);
	res = parse_components(world, components, variables,
		ECS_MAX_COMPONENTS - 1);
	va_end(variables);
	if (res == false)
		return (-1);
	else
		res = world->next_free_entity_index;
	world->entities[world->next_free_entity_index] = components;
	if (world->next_vacant_entity_index != -1)
		world->next_free_entity_index =
			world->vacant_entities[world->next_vacant_entity_index--];
	else
		world->next_free_entity_index++;
	world->num_entities++;
	return (res);
}

uint64_t		world_entity_get(t_world *world, uint64_t entity)
{
	return (world->entities[entity]);
}

static void		world_entity_remove_component(t_world *world,
				uint64_t entity_index, uint64_t component_id)
{
	uint64_t		component_list_index;

	component_list_index =
		get_component_list_index(world, component_id);
	hash_map_delete(world->component_list[component_list_index],
		entity_index);
}

void			world_entity_remove(t_world *world, uint64_t entity_index)
{
	uint64_t		entity_components;
	uint64_t		component_id;
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
			world_entity_remove_component(world, entity_index, component_id);
		entity_components >>= 1;
		shift--;
	}
	world->entities[entity_index] = ECS_EMPTY_ENTITY;
	world->next_vacant_entity_index++;
	world->vacant_entities[world->next_vacant_entity_index] = entity_index;
	world->num_entities--;
}

t_bool			world_entity_valid(t_world *world,
				uint64_t entity_index)
{
	return (world->entities[entity_index] > ECS_EMPTY_ENTITY);
}

void			world_entity_components_add(t_world *world,
				uint64_t entity_index, uint64_t components, ...)
{
	va_list		variables;
	uint64_t	entity_mask;
	t_bool		res;
	
	if (!world_entity_valid(world, entity_index))
		return ;
	va_start(variables, components);
	entity_mask = components;
	res = parse_components(world, components, variables, entity_index);
	va_end(variables);
}

void			world_entity_component_remove(t_world *world,
				uint64_t entity_index, uint64_t component)
{
	if (!world_entity_valid(world, entity_index))
		return ;
	world_entity_remove_component(world, entity_index, component);
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

void			*world_entity_component_get(t_world *world,
				uint64_t entity_index, uint64_t component)
{
	uint64_t	component_index;

	if (!world_entity_valid(world, entity_index) ||
		!hash_map_has_key(world->component_to_list, component))
		return (NULL);
	component_index = get_component_list_index(world, component);
	return (hash_map_get(world->component_list[component_index], entity_index));	
}