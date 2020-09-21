/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   demo_system.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 19:20:36 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/21 12:17:40 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "demo.h"

static void					system_fall_handle(t_ecs_world *world,
							uint64_t entity_index)
{
	t_app			*app;
	t_position		*pos;
	t_velocity		*vel;
	uint32_t		dt;

	app = (t_app*)world->systems[ecs_system_index(world, system_fall)].params;
	dt = app->info.delta_time;
	pos = (t_position*)hash_map_get(ecs_component_entities(world, comp_pos),
		entity_index);
	vel = (t_velocity*)hash_map_get(ecs_component_entities(world, comp_vel),
		entity_index);
	if (pos && vel)
		pos->y += vel->dy * dt;
}

static void					system_render_handle(t_ecs_world *world,
							uint64_t entity_index)
{

	t_visuals		*render_specs;
	t_window		*window;
	t_position		*pos;
	int32_t			x_start;
	int32_t			y_start;
	int32_t			x;
	int32_t			y;

	window = ((t_app*)world->systems[
		ecs_system_index(world, system_fall)].params)->window;
	render_specs =
		(t_visuals*)hash_map_get(ecs_component_entities(world, comp_vis),
			entity_index);
	pos = (t_position*)hash_map_get(ecs_component_entities(world, comp_pos),
			entity_index);
	x_start = pos->x - render_specs->width / 2;
	y_start = pos->y - render_specs->height / 2;
	y = y_start - 1;
	while (++y < y_start + (int)render_specs->height)
	{
		x = pos->x - render_specs->width / 2 - 1;
		while (++x < x_start + (int)render_specs->width)
		{
			if (y >= 0 && x >= 0 && y < window->height && x < window->width)
			{
				window->framebuffer[y * window->width + x] =
					render_specs->color;
			}
		}
	}
}

static void					system_remove_handle(t_ecs_world *world,
							uint64_t entity_index)
{
	t_app			*app;
	t_visuals		*render_specs;
	t_position		*pos;

	app = (t_app*)world->systems[ecs_system_index(world, system_fall)].params;
	render_specs =
		(t_visuals*)hash_map_get(ecs_component_entities(world, comp_vis),
		entity_index);
	pos = (t_position*)hash_map_get(ecs_component_entities(world, comp_pos),
		entity_index);
	if (pos && render_specs &&
		pos->y > app->window->height + render_specs->height / 2)
	{
		ecs_world_entity_remove(world, entity_index);
		ft_printf("Removed entity id: %d, entities: %d\n",
			entity_index, world->num_entities);
	}
}

/*
** Define systems & their param initial values
*/

void						systems_create(t_app *app)
{
	ecs_world_system_add(app->world, (t_system){
		.system_id = system_render,
		.components_mask = comp_vis | comp_pos,
		.system_handle_func = system_render_handle,
		.params = app
	});
	ecs_world_system_add(app->world, (t_system){
		.system_id = system_fall,
		.components_mask = comp_pos | comp_vel,
		.system_handle_func = system_fall_handle,
		.params = app
	});
	ecs_world_system_add(app->world, (t_system){
		.system_id = system_remove,
		.components_mask = comp_pos | comp_vis,
		.system_handle_func = system_remove_handle,
		.params = app
	});
}

/*
** Update system params, e.g. delta_time for velocity calculations
*/

void						systems_params_update(t_app *app)
{
	ecs_system_update_params(app->world, system_fall, app);
}
