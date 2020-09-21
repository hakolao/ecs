/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   demo_entities.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 19:22:57 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/21 17:00:18 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "demo.h"

static void					random_entity_create(t_app *app, uint32_t z_val)
{
	int32_t			size;
	t_physics		physics;

	size = ENTITY_SIZE;
	physics.position.x =
		(float)((size / 2) + rand() % app->window->width) - (float)(size / 2);
	physics.position.y =
		(float)((float)size / 2.0 + rand() % 30) - (float)(size / 2) - 150.0;
	physics.mass = ft_abs((rand() / 2 * rand()) % 1000) * 0.01 + 1.0;
	physics.mass = physics.mass == 0.0 ? physics.mass + 0.2 : physics.mass;
	physics.inertia = 1.0;
	physics.velocity.dx = 0.0;
	physics.velocity.dy = 0.0;
	ecs_world_entity_add(app->world, 2,
		&(t_component){.id = comp_physics, .size = sizeof(t_physics),
			.data = &physics},
		&(t_component){.id = comp_vis, .size = sizeof(t_visuals),
			.data = &(t_visuals){.color = rand(), .z_val = z_val,
				.width = size, .height = size}});
}

/*
** Create some entities with randomized params :)
*/

void						entities_create_up_to_max(t_app *app)
{
	int		z_val;

	z_val = 0;
	while (app->world->num_entities < MAX_ENTITIES)
		random_entity_create(app, z_val++);
}
