/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   demo_entities.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 19:22:57 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/22 13:24:28 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "demo.h"

void						init_entity(t_app *app, t_physics *physics,
							t_visuals *visuals)
{
	uint32_t	size;

	size = rand() % ENTITY_SIZE + 1;
	physics->position.x = (float)(rand() % app->window->width);
	physics->position.y = (float)(rand() % app->window->height) -
		app->window->height;
	physics->mass = ft_abs((rand() / 2 * rand()) % 1000) * 0.01 + 1.0;
	physics->mass = physics->mass == 0.0 ? physics->mass + 0.2 : physics->mass;
	physics->inertia = 1.0;
	physics->velocity.dx = 0.0;
	physics->velocity.dy = 0.0;
	visuals->color = ((rand() % 255) << 24) | ((rand() % 255) << 16) |
		((rand() % 255) << 8) | ((rand() % 255) << 24);
	visuals->width = size;
	visuals->height = size;
	visuals->z_val = rand();
}

static void					random_entity_create(t_app *app)
{
	t_physics		physics;
	t_visuals		visuals;

	init_entity(app, &physics, &visuals);
	ecs_world_entity_add(app->world, 2,
		&(t_component){.id = comp_physics, .size = sizeof(t_physics),
			.data = &physics},
		&(t_component){.id = comp_vis, .size = sizeof(t_visuals),
			.data = &visuals});
}

/*
** Create some entities with randomized params :)
*/

void						entities_create_up_to_max(t_app *app)
{
	while (app->world->num_entities < MAX_ENTITIES)
		random_entity_create(app);
}
