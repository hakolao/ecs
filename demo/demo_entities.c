/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   demo_entities.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 19:22:57 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/21 12:18:58 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "demo.h"

/*
** Create some entities with randomized params :)
*/

void						entities_create(t_app *app)
{
	int32_t		i;
	int32_t		size;

	i = 1;
	while (app->world->num_entities < MAX_ENTITIES)
	{
		size = 2;
		ecs_world_entity_add(app->world, 3,
			&(t_component){.id = comp_pos, .size = sizeof(t_position),
				.data = &(t_position){
					.x = (float)((size / 2) + i % WIDTH) - (float)(size / 2),
					.y = (float)((float)size / 2.0 + (ft_rand(i) % 30)) -
						(float)(size / 2)}},
			&(t_component){.id = comp_vel, .size = sizeof(t_velocity),
				.data = &(t_velocity){.dx = 0., .dy = 0.1}},
			&(t_component){.id = comp_vis, .size = sizeof(t_visuals),
				.data = &(t_visuals){.color = i,
						.width = size, .height = size}});
		i = ft_rand(i);
	}
}
