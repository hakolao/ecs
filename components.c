/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   components.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/15 23:41:29 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/16 14:16:18 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libecs.h"

void					world_component_add(t_world *world,
						uint64_t component)
{
	uint64_t	next_free_index;
	void		*get_res;
	uint64_t	mapped_index;

	if (hash_map_has_key(world->component_to_list, component))
	{
		get_res = hash_map_get(world->component_to_list, component);
		if (get_res != NULL)
			mapped_index = *(uint64_t*)&get_res;
		else
			mapped_index = 0;
		if (world->component_list[mapped_index] != NULL)
			return ;
	}
	world->component_list[world->next_free_component_index] =
		hash_map_create(world->max_entities);
	hash_map_add(world->component_to_list, component,
		(void*)world->next_free_component_index);
	next_free_index = world->next_free_component_index + 1;
	while (next_free_index < world->num_components &&
		world->component_list[next_free_index] != NULL)
		next_free_index++;
	world->next_free_component_index = next_free_index;
	world->num_components++;
}

void					world_component_remove(t_world *world,
						uint64_t component)
{
	uint64_t	i;
	uint64_t	removed;
	void		*get_res;

	i = -1;
	removed = 0;
	get_res = hash_map_get(world->component_to_list, component);
	if (hash_map_has_key(world->component_to_list, component))
	{
		while (++i < world->num_components + removed)
		{
			if (i == (get_res == NULL ? 0 : *(uint64_t*)&get_res))
			{
				hash_map_destroy_free(world->component_list[i]);
				hash_map_delete(world->component_to_list, component);
				world->component_list[i] = NULL;
				world->next_free_component_index =
					i < world->next_free_component_index ?
					i : world->next_free_component_index;
				world->num_components--;
			}
			else if (world->component_list[i] == NULL)
				removed++;
		}
	}
}
