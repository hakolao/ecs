/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libecs_internal_entity_utils.h                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/16 17:06:11 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/17 17:03:13 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ECS_INTERNAL_ENTITY_UTILS_H
# define ECS_INTERNAL_ENTITY_UTILS_H

#include "libecs.h"

uint64_t		get_component_list_index(t_ecs_world *world, uint64_t component_id);
uint64_t		parse_components(t_ecs_world *world, uint64_t num_components,
				va_list variables, uint64_t entity_index);
t_bool			ecs_world_entity_valid(t_ecs_world *world,
				uint64_t entity_index);
size_t			get_component_size(t_ecs_world *world,
				uint64_t component_list_index);
void			entity_remove_component(t_ecs_world *world,
				uint64_t entity_index, uint64_t component_id);
t_bool			is_valid_component_id(uint64_t component_id);

#endif
