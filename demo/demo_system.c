/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   demo_system.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 19:20:36 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/18 14:16:48 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "demo.h"

static void					system_movement_handle(t_ecs_world *world,
							uint64_t entity_index)
{
	void			*system_args;
	t_position		*pos;
	t_velocity		*vel;
	uint32_t		dt;

	system_args = world->systems[ecs_system_index(world, system_move)].params;
	if (system_args != NULL)
	{
		dt = (uint32_t)(*(size_t*)system_args);
		pos = (t_position*)hash_map_get(ecs_component_entities(world, comp_pos),
			entity_index);
		vel = (t_velocity*)hash_map_get(ecs_component_entities(world, comp_vel),
			entity_index);
		if (pos && vel)
		{
			pos->x += vel->dx * dt;
			pos->y += vel->dy * dt;
		}
	}
}

static void					system_render_handle(t_ecs_world *world,
							uint64_t entity_index)
{

	t_render		*render_specs;
	t_window		*window;
	t_position		*pos;
	int32_t			x_start;
	int32_t			y_start;
	int32_t			x;
	int32_t			y;

	render_specs =
		(t_render*)hash_map_get(ecs_component_entities(world, comp_render),
			entity_index);
	pos = (t_position*)hash_map_get(ecs_component_entities(world, comp_pos),
			entity_index);
	window = (*render_specs->window);
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

/*
** Define systems & their param initial values
*/

void						systems_create(t_app *app)
{
	ecs_world_system_add(app->world, (t_system){
		.system_id = system_render,
		.components_mask = comp_render | comp_pos,
		.system_handle_func = system_render_handle,
		.params = NULL
	});
	ecs_world_system_add(app->world, (t_system){
		.system_id = system_move,
		.components_mask = comp_pos | comp_vel,
		.system_handle_func = system_movement_handle,
		.params = (void*)&app->delta_time
	});
}

/*
** Update system params, e.g. delta_time for velocity calculations
*/

void						systems_params_update(t_app *app)
{
	ecs_system_update_params(app->world, system_move, &app->delta_time);
}
