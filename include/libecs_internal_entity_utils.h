/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libecs_internal_entity_utils.h                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/16 17:06:11 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/16 17:17:11 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ECS_INTERNAL_ENTITY_UTILS_H
# define ECS_INTERNAL_ENTITY_UTILS_H

#include "libecs.h"

uint64_t		get_component_list_index(t_world *world, uint64_t component_id);
uint64_t		component_mask(uint64_t components, uint64_t shift);
t_bool			parse_components(t_world *world, uint64_t components,
				va_list variables, uint64_t entity_index);
t_bool			world_entity_valid(t_world *world,
				uint64_t entity_index);
size_t			get_component_size(t_world *world,
				uint64_t component_list_index);
void			entity_remove_component(t_world *world,
				uint64_t entity_index, uint64_t component_id);

#endif