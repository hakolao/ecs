/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   demo_system.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 19:20:36 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/24 12:29:16 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "demo_raycast.h"

static void					system_render_handle(t_ecs_world *world,
							uint64_t entity_index)
{
	t_app			*app;
	t_ray			*ray;
	t_pixel			*pixel;
	t_demo_data		*data;
	uint32_t		i;

	app = (t_app*)world->systems[ecs_system_index(world, system_render)].params;
	data = (t_demo_data*)app->data;
	ray = (t_ray*)ecs_world_entity_component_get(world,
		entity_index, comp_ray);
	pixel = (t_pixel*)ecs_world_entity_component_get(world,
		entity_index, comp_pixel);
	i = -1;
	while (++i < data->num_objects)
	{
		// if (kd_tree_ray_hit(data->scene[i]->triangle_tree->root, ray,
		// 	1000.0, hit_point))
		// 	app->window->framebuffer[pixel->y * app->window->width + pixel->x] =
		// 		0x00FF00FF;
		for (int32_t j = 0; j < data->objects[i]->num_triangles; j++)
		{
			t_vec3 		hitp;
			if (kd_tree_triangle_hit(&data->objects[i]->triangles[j], ray, hitp))
				app->window->framebuffer[pixel->y * app->window->width + pixel->x] =
					0xFF0000FF;
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
