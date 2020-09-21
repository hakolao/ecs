/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   demo_entities.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 19:22:57 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/21 14:01:28 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "demo.h"

static void					random_entity_create(t_app *app)
{
	int32_t			size;
	t_physics		physics;

	size = 2;
	physics.position.x =
		(float)((size / 2) + rand() % WIDTH) - (float)(size / 2);
	physics.position.y =
		(float)((float)size / 2.0 + rand() % 30) - (float)(size / 2) - 50.0;
	physics.mass = ft_abs((rand() / 2 * rand()) % 1000) * 0.01 + 1.0;
	physics.mass = physics.mass == 0.0 ? physics.mass + 0.2 : physics.mass;
	physics.inertia = 1.0;
	physics.velocity.dx = 0.0;
	physics.velocity.dy = 0.0;
	ecs_world_entity_add(app->world, 2,
		&(t_component){.id = comp_physics, .size = sizeof(t_physics),
			.data = &physics},
		&(t_component){.id = comp_vis, .size = sizeof(t_visuals),
			.data = &(t_visuals){.color = rand(),
					.width = size, .height = size}});
}

/*
** Create some entities with randomized params :)
*/

void						entities_create(t_app *app)
{
	while (app->world->num_entities < MAX_ENTITIES)
		random_entity_create(app);
}
