/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libecs_internals.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/01 16:23:40 by ohakola           #+#    #+#             */
/*   Updated: 2020/10/01 16:25:44 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBECS_INTERNALS_H
# define LIBECS_INTERNALS_H

# include "libecs.h"

uint64_t		parse_components(t_ecs_world *world, uint64_t num_components,
				va_list variables, uint64_t entity_index);
size_t			get_component_size(t_ecs_world *world,
				uint64_t component_list_index);
void			entity_remove_component(t_ecs_world *world,
				uint64_t entity_index, uint64_t component_id);
t_bool			is_valid_component_id(uint64_t component_id);
uint64_t		ecs_world_new_entity_index(t_ecs_world *world);
t_bool			contains_system(uint64_t systems_group, uint64_t system);
t_bool			system_and_entity_matches(t_ecs_world *world,
				uint64_t systems, uint64_t entity_index, uint64_t system_index);

#endif
