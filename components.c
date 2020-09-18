/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   components.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/15 23:41:29 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/18 13:58:16 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libecs.h"

/*
** https://stackoverflow.com/questions/51094594/
*/

t_bool					is_valid_component_id(uint64_t component_id)
{
	return (component_id && !(component_id & (component_id - 1)));
}

static t_bool			is_valid_component(t_ecs_world *world,
						uint64_t component)
{
	if (!is_valid_component_id(component))
		return (false);
	return (ecs_component_entities(world, component) == NULL);
}

t_hash_table			*ecs_component_entities(t_ecs_world *world,
						uint64_t component_id)
{
	if (!hash_map_has_key(world->component_to_index, component_id))
		return (NULL);
	return (world->components_to_entity[
			ecs_component_index(world, component_id)]);
}

void					ecs_world_component_add(t_ecs_world *world,
						uint64_t component)
{
	uint64_t	next_free_index;

	if (!is_valid_component(world, component))
		return ;
	world->components_to_entity[world->next_free_component_index] =
		hash_map_create(world->max_entities);
	hash_map_add(world->component_to_index, component,
		(void*)world->next_free_component_index);
	next_free_index = world->next_free_component_index + 1;
	while (next_free_index < world->num_components &&
		world->components_to_entity[next_free_index] != NULL)
		next_free_index++;
	world->next_free_component_index = next_free_index;
	world->num_components++;
}

void					ecs_world_component_remove(t_ecs_world *world,
						uint64_t component)
{
	uint64_t	i;
	uint64_t	removed;
	void		*get_res;

	i = -1;
	removed = 0;
	get_res = hash_map_get(world->component_to_index, component);
	if (hash_map_has_key(world->component_to_index, component))
	{
		while (++i < world->num_components + removed)
		{
			if (i == (get_res == NULL ? 0 : *(uint64_t*)&get_res))
			{
				hash_map_destroy_free(world->components_to_entity[i]);
				hash_map_delete(world->component_to_index, component);
				world->components_to_entity[i] = NULL;
				world->next_free_component_index =
					i < world->next_free_component_index ?
					i : world->next_free_component_index;
				world->num_components--;
			}
			else if (world->components_to_entity[i] == NULL)
				removed++;
		}
	}
}
