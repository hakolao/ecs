/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   demo_system.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 19:20:36 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/21 21:13:19 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "demo.h"

static void					system_forces_handle(t_ecs_world *world,
							uint64_t entity_index)
{
	t_app			*app;
	t_physics		*physics;
	float			dt;
	t_vel			forces;

	app = (t_app*)world->systems[ecs_system_index(world, system_forces)].params;
	dt = (float)app->info.delta_time * 0.02;
	physics = (t_physics*)hash_map_get(ecs_component_entities(world, comp_physics),
		entity_index);
	if (!physics || !app->is_gravity)
		return ;
	forces = (t_vel){.dx = 0, .dy = 0};
	forces.dy += CONST_GRAVITY;
	forces.dx += physics->velocity.dx * CONST_MOVEMENT;
	forces.dy += physics->velocity.dy * CONST_MOVEMENT;
	physics->position.x += forces.dx * dt / physics->mass;
	physics->position.y += forces.dy * dt / physics->mass;
}

static void					system_render_handle(t_ecs_world *world,
							uint64_t entity_index)
{

	t_visuals		*render_specs;
	t_window		*window;
	t_physics		*physics;
	int32_t			x_start;
	int32_t			y_start;
	int32_t			x;
	int32_t			y;
	uint32_t		pixel_index;

	window = ((t_app*)world->systems[
		ecs_system_index(world, system_forces)].params)->window;
	render_specs =
		(t_visuals*)hash_map_get(ecs_component_entities(world, comp_vis),
			entity_index);
	physics = (t_physics*)hash_map_get(ecs_component_entities(world, comp_physics),
			entity_index);
	x_start = physics->position.x - render_specs->width / 2;
	y_start = physics->position.y - render_specs->height / 2;
	y = y_start - 1;
	while (++y < y_start + (int)render_specs->height)
	{
		x = x_start - 1;
		while (++x < x_start + (int)render_specs->width)
		{
			if (y >= 0 && x >= 0 && y < window->height && x < window->width)
			{
				pixel_index = y * window->width + x;
				if (render_specs->z_val <=
					window->zbuffer[pixel_index])
				{
					window->framebuffer[pixel_index] = render_specs->color;
					window->zbuffer[pixel_index] = render_specs->z_val;
				}
			}
		}
	}
}

static void					system_reset_handle(t_ecs_world *world,
							uint64_t entity_index)
{
	t_app			*app;
	t_visuals		*render_specs;
	t_physics		*physics;

	app = (t_app*)world->systems[ecs_system_index(world, system_forces)].params;
	render_specs =
		(t_visuals*)hash_map_get(ecs_component_entities(world, comp_vis),
		entity_index);
	physics = (t_physics*)hash_map_get(ecs_component_entities(world, comp_physics),
		entity_index);
	if (physics && render_specs &&
		physics->position.y > app->window->height + render_specs->height / 2)
		init_entity_phyiscs(app, physics);
}

/*
** Define systems & their param initial values
*/

void						systems_create(t_app *app)
{
	ecs_world_system_add(app->world, (t_system){
		.system_id = system_render,
		.components_mask = comp_vis | comp_physics,
		.system_handle_func = system_render_handle,
		.params = app
	});
	ecs_world_system_add(app->world, (t_system){
		.system_id = system_forces,
		.components_mask = comp_physics,
		.system_handle_func = system_forces_handle,
		.params = app
	});
	ecs_world_system_add(app->world, (t_system){
		.system_id = system_reset,
		.components_mask = comp_physics | comp_vis,
		.system_handle_func = system_reset_handle,
		.params = app
	});
}

/*
** Update system params, e.g. delta_time for velocity calculations
*/

void						systems_params_update(t_app *app)
{
	ecs_system_update_params(app->world, system_forces, app);
}
