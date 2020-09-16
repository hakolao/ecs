/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entities.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/15 23:41:23 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/16 17:16:58 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libecs.h"
#include "libecs_internal_entity_utils.h"

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

void			world_entity_remove(t_world *world, uint64_t entity_index)
{
	uint64_t		entity_components;
	uint64_t		component_id;
	int				shift;

	if (!world_entity_valid(world, entity_index))
		return ;
	entity_components = world_entity_get(world, entity_index);
	shift = ECS_MAX_COMPONENTS - 1;
	while (entity_components != 0)
	{
		component_id = (1ULL << shift)	& entity_components;
		if (component_id != 0)
			entity_remove_component(world, entity_index, component_id);
		entity_components >>= 1;
		shift--;
	}
	world->entities[entity_index] = ECS_EMPTY_ENTITY;
	world->next_vacant_entity_index++;
	world->vacant_entities[world->next_vacant_entity_index] = entity_index;
	world->num_entities--;
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
	entity_remove_component(world, entity_index, component);
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