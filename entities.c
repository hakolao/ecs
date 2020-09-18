/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entities.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/15 23:41:23 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/18 14:05:57 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libecs.h"
#include "libecs_internal_entity_utils.h"

/*
** Adding an entity fills the first vacant index. If no vacancies next free
** entity index is used.
** Num components defines how many va_args are parsed, which should be of
** t_component type. See tests for more information.
*/

int64_t			ecs_world_entity_add(t_ecs_world *world,
				uint64_t num_components, ...)
{
	va_list			variables;
	uint64_t		entity_mask;
	uint64_t		new_entity_index;

	if (world->next_vacancy_index > -1)
	{
		new_entity_index = world->vacant_entities[world->next_vacancy_index];
		world->vacant_entities[world->next_vacancy_index] = 0;
		world->next_vacancy_index--;
	}
	else
		new_entity_index = world->next_free_entity_index++;
	if (new_entity_index >= world->max_entities &&
		ft_dprintf(2, "Entity index %d over max entities %d, entity not added",
			new_entity_index, world->max_entities))
		return (-1);
	va_start(variables, num_components);
	entity_mask = parse_components(world, num_components, variables,
		new_entity_index);
	va_end(variables);
	if (entity_mask == false && ft_dprintf(2, "Failed to add entity\n"))
		return (-1);
	world->entities[new_entity_index] = entity_mask;
	world->num_entities++;
	return (new_entity_index);
}

/*
** Loop through each valid componentid (enums as bits, up to 64),
** and if entity has a matching component that entity's component gets removed
** from the component by entity hash map.
** Removed entity in world->entities becomes empty and next vacancy index
** gets incremented and vacant entity gets set to be the empty entity's index.
*/

void			ecs_world_entity_remove(t_ecs_world *world,
				uint64_t entity_index)
{
	uint64_t		entity_components;
	uint64_t		component_id;
	int				shift;

	if (!ecs_world_entity_valid(world, entity_index))
		return ;
	entity_components = ecs_world_entity_get(world, entity_index);
	shift = 0;
	while (shift < ECS_MAX_COMPONENTS - 1)
	{
		component_id = (1ULL << shift) & entity_components;
		if (component_id)
			entity_remove_component(world, entity_index, component_id);
		shift++;
	}
	world->entities[entity_index] = ECS_EMPTY_ENTITY;
	world->next_vacancy_index++;
	world->vacant_entities[world->next_vacancy_index] = entity_index;
	world->num_entities--;
}

/*
** An existing entity may get new components via this function. This works like
** when adding an entity, but instead entity_index has been given as input.
** parse components replaces matching components that already exist.
*/

void			ecs_world_entity_components_add(t_ecs_world *world,
				uint64_t entity_index, uint64_t num_components, ...)
{
	va_list		variables;
	uint64_t	added_components;

	if (!ecs_world_entity_valid(world, entity_index))
		return ;
	va_start(variables, num_components);
	added_components =
		parse_components(world, num_components, variables, entity_index);
	world->entities[entity_index] |= added_components;
	va_end(variables);
}

/*
** Example of how component removal works:
** Components to remove 100
** entity: 1111
** component 100
** after 1011
*/

void			ecs_world_entity_components_remove(t_ecs_world *world,
				uint64_t entity_index, uint64_t components_to_remove)
{
	uint64_t		entity_components;
	uint64_t		curr_component;
	int				shift;

	if (!ecs_world_entity_valid(world, entity_index))
		return ;
	entity_components = ecs_world_entity_get(world, entity_index);
	shift = 0;
	while (shift < ECS_MAX_COMPONENTS - 1)
	{
		curr_component = (1ULL << shift) & components_to_remove;
		if (curr_component &&
			ecs_world_entity_at_contains(world, entity_index, curr_component))
		{
			entity_remove_component(world, entity_index, curr_component);
			entity_components ^= curr_component;
		}
		shift++;
	}
	if (entity_components == 0)
	{
		ecs_world_entity_remove(world, entity_index);
		return ;
	}
	world->entities[entity_index] = entity_components;
}

/*
** Fetches the component of an entity as void*. The caller should know how to
** cast the result.
*/

void			*ecs_world_entity_component_get(t_ecs_world *world,
				uint64_t entity_index, uint64_t component)
{
	uint64_t	component_index;

	if (!ecs_world_entity_valid(world, entity_index) ||
		!hash_map_has_key(world->index_by_component, component))
		return (NULL);
	component_index = ecs_component_index(world, component);
	return (hash_map_get(world->components_by_entity[component_index],
			entity_index));
}
