/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   demo_system.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 19:20:36 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/27 19:45:37 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "demo_raycast.h"

static uint32_t				color(t_ray *ray)
{
	t_vec3	unit_dir;
	float	t;

	ml_vector3_normalize(ray->dir, unit_dir);
	t = 0.5 * (unit_dir[1] + 1.0);
	return (color_blend_u32(rgba_to_u32((SDL_Color){255, 255, 255, 255}),
		rgba_to_u32((SDL_Color){0.5 * 255, 0.7 * 255, 1.0 * 255, 255}),
		t));
}

static void					system_render_handle(t_ecs_world *world,
							uint64_t entity_index)
{
	t_app			*app;
	t_ray			*ray;
	t_pixel			*pixel;
	t_demo_data		*data;
	float			t;
	uint32_t		i;

	app = (t_app*)world->systems[ecs_system_index(world, system_render)].params;
	data = (t_demo_data*)app->data;
	ray = (t_ray*)ecs_world_entity_component_get(world,
		entity_index, comp_ray);
	pixel = (t_pixel*)ecs_world_entity_component_get(world,
		entity_index, comp_pixel);
	app->window->framebuffer[pixel->y * app->window->width + pixel->x] =
		color(ray);
	i = -1;
	while (++i < data->num_objects)
	{
		if (kd_tree_ray_hit(data->objects[i]->triangle_tree->root, ray, &t))
		{
			app->window->framebuffer[pixel->y * app->window->width + pixel->x] =
				0x0000FFFF;
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
		.components_mask = comp_ray | comp_pixel,
		.system_handle_func = system_render_handle,
		.params = app
	});
}

void						systems_params_update(t_app *app)
{
	ecs_system_update_params(app->world, system_render, app);
}
