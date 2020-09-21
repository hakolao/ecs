/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   demo_entities.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 19:22:57 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/21 13:01:58 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "demo.h"

/*
** Create some entities with randomized params :)
*/

void						entities_create(t_app *app)
{
	int32_t			i;
	int32_t			size;
	t_physics	physics;

	i = 1;
	while (app->world->num_entities < MAX_ENTITIES)
	{
		size = 2;
		physics.mass = 1.0;
		physics.inertia = 1.0;
		physics.position.x =
			(float)((size / 2) + i % WIDTH) - (float)(size / 2);
		physics.position.y =
			(float)((float)size / 2.0 + (ft_rand(i) % 30)) - (float)(size / 2);
		physics.velocity.dx = 0.0;
		physics.velocity.dy = 0.0;
		ecs_world_entity_add(app->world, 2,
			&(t_component){.id = comp_physics, .size = sizeof(t_physics),
				.data = &physics},
			&(t_component){.id = comp_vis, .size = sizeof(t_visuals),
				.data = &(t_visuals){.color = i,
						.width = size, .height = size}});
		i = ft_rand(i);
	}
}
