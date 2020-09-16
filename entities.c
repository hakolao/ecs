/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entities.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/15 23:41:23 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/16 15:31:35 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libecs.h"

// uint64_t		world_entity_add(t_world *world, uint64_t components, ...)
// {
// 	va_list		variables;
// 	void		*val;
// 	uint64_t	component_id;
// 	void		*get_res;
// 	uint64_t	entity_mask;
// 	int			shift;
	
// 	va_start(variables, components);
// 	entity_mask = components;
// 	shift = ECS_MAX_COMPONENTS - 1;
// 	while (components != 0)
// 	{
// 		val = va_arg(variables, void *);
// 		component_id = (1ULL << shift) & components;
// 		if (!hash_map_has_key(world->component_to_list, component_id))
// 			world_component_add(world, component_id, val);
// 		get_res = hash_map_get(world->component_to_list, component_id);
// 		hash_map_add(
// 			world->component_list[get_res == 0 ? 0 : *(uint64_t*)&get_res],
// 			entity_mask, val);
// 		components >>= 1;
// 		shift--;
// 	}
// 	va_end(variables);
// 	// world->entities[world->next_free_entity_index] = components;
// }

// void			world_entity_component_add(t_world *world,
// 				uint64_t entity, uint64_t component)
// {

// }

// void			world_entity_remove(t_world *world, uint64_t entity)
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