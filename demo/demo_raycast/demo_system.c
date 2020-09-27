/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   demo_system.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 19:20:36 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/27 21:20:47 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "demo_raycast.h"

static uint32_t				average_color(uint32_t *colors, uint32_t num_colors)
{
	int			i;
	SDL_Color	color;
	uint32_t	rgba[4];

	i = -1;
	while (++i < 4)
		rgba[i] = 0;
	i = -1;
	while (++i < (int)num_colors)
	{
		color = u32_to_rgba(colors[i]);
		rgba[0] += color.r;
		rgba[1] += color.g;
		rgba[2] += color.b;
		rgba[3] += color.a;
	}
	return (rgba_to_u32((SDL_Color){
		rgba[0] / num_colors,
		rgba[1] / num_colors,
		rgba[2] / num_colors,
		rgba[3] / num_colors
	}));
}

static uint32_t				color_default(t_ray *ray)
{
	t_vec3	unit_dir;
	float	t;

	ml_vector3_normalize(ray->dir, unit_dir);
	t = 0.5 * (unit_dir[1] + 1.0);
	return (color_blend_u32(rgba_to_u32((SDL_Color){255, 255, 255, 255}),
		rgba_to_u32((SDL_Color){0.5 * 255, 0.7 * 255, 1.0 * 255, 255}),
		t));
}

static uint32_t				color_hit(t_ray *ray, t_hit *hit)
{
	t_vec3	from_camera;

	ml_vector3_sub(hit->hit_point, ray->origin, from_camera);
	ml_vector3_normalize(from_camera, from_camera);
	return (color_blend_u32(color_default(ray),
		rgba_to_u32((SDL_Color){0, 0, 0, 255}),
		from_camera[2]));
}

static void					system_render_handle(t_ecs_world *world,
							uint64_t entity_index)
{
	t_app			*app;
	t_ray			*rays;
	t_pixel			*pixel;
	t_demo_data		*data;
	t_hit			hit;
	uint32_t		i;
	int32_t			j;
	uint32_t		colors[((t_demo_data*)(
		(t_app*)world->systems[ecs_system_index(world, system_render)].params
	)->data)->ray_samples];

	app = (t_app*)world->systems[ecs_system_index(world, system_render)].params;
	data = (t_demo_data*)app->data;
	rays = (t_ray*)ecs_world_entity_component_get(world,
		entity_index, comp_ray);
	pixel = (t_pixel*)ecs_world_entity_component_get(world,
		entity_index, comp_pixel);
	i = -1;
	while (++i < data->num_objects)
	{
		j = -1;
		while (++j < data->ray_samples)
		{
			if (kd_tree_ray_hit(data->objects[i]->triangle_tree->root, &rays[i],
				FLT_MAX, &hit))
			{
				colors[j] = color_hit(&rays[i], &hit);
			}
			else
				colors[j] = color_default(&rays[i]);
		}
	}
	app->window->framebuffer[pixel->y * app->window->width + pixel->x] =
		average_color(colors, data->ray_samples);
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
