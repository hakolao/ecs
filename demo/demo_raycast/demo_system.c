/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   demo_system.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 19:20:36 by ohakola           #+#    #+#             */
/*   Updated: 2020/10/07 15:37:14 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "demo_raycast.h"

static uint32_t				average_color(uint32_t *colors, float num_colors)
{
	int			i;
	uint32_t	color[4];
	uint32_t	rgba[4];

	i = -1;
	while (++i < 4)
		rgba[i] = 0;
	i = -1;
	while (++i < (int)num_colors)
	{
		l3d_u32_to_rgba(colors[i], color);
		rgba[0] += color[0];
		rgba[1] += color[1];
		rgba[2] += color[2];
		rgba[3] += color[3];
	}
	return (l3d_rgba_to_u32((uint32_t[4]){
		255 * sqrt((float)(rgba[0] / num_colors) / 255.0),
		255 * sqrt((float)(rgba[1] / num_colors) / 255.0),
		255 * sqrt((float)(rgba[2] / num_colors) / 255.0),
		255 * sqrt((float)(rgba[3] / num_colors) / 255.0)
	}));
}

static uint32_t				color_default(t_ray *ray)
{
	t_vec3	unit_dir;
	float	t;

	ml_vector3_normalize(ray->dir, unit_dir);
	t = 0.5 * (unit_dir[1] + 1.0);
	return (l3d_color_blend_u32(
		l3d_rgba_to_u32((uint32_t[4]){255, 255, 255, 255}),
		l3d_rgba_to_u32((uint32_t[4]){0.5 * 255, 0.7 * 255, 1.0 * 255, 255}),
		t));
}

void						random_in_unit_sphere(t_vec3 res)
{
	t_vec3	orig;
	t_vec3	rand;
	float	p_sqrd;

	ml_vector3_set(res, 0, 0, 0);
	ml_vector3_set(orig, 1.0, 1.0, 1.0);
	ml_vector3_set(rand, l3d_rand_d(), l3d_rand_d(), l3d_rand_d());
	ml_vector3_mul(rand, 2.0, rand);
	ml_vector3_sub(rand, orig, res);
	p_sqrd = 2 * res[0] * res[0] + 2 * res[1] * res[1] + 2 * res[2] * res[2];
	while (p_sqrd >= 1.0)
	{
		ml_vector3_set(rand, l3d_rand_d(), l3d_rand_d(), l3d_rand_d());
		ml_vector3_mul(rand, 2.0, rand);
		ml_vector3_sub(rand, orig, res);
		p_sqrd = 2 * res[0] * res[0] + 2 * res[1] * res[1] + 2 * res[2] * res[2];
	}
}

/*
** https://www.realtimerendering.com/raytracing/Ray%20Tracing%20in%20a%20Weekend.pdf
*/

static uint32_t				color(t_kd_node *root, t_ray *ray, uint32_t bounces)
{
	t_hit			hit;
	t_vec3			target;
	t_vec3			random;
	t_ray			new_ray;
	t_vec3			direction;
	uint32_t		new_color[4];

	if (bounces < MAX_BOUNCES && l3d_kd_tree_ray_hit(root, ray, FLT_MAX, &hit))
	{
		random_in_unit_sphere(random);
		ml_vector3_add(hit.hit_point, hit.normal, target);
		ml_vector3_add(target, random, target);
		ml_vector3_sub(target, hit.hit_point, direction);
		l3d_ray_set(direction, hit.hit_point, &new_ray);
		l3d_u32_to_rgba(color(root, &new_ray, bounces + 1), new_color);
		new_color[0] *= 0.5;
		new_color[1] *= 0.5;
		new_color[2] *= 0.5;
		new_color[3] *= 0.5;
		return (l3d_rgba_to_u32(new_color));
	}
	return (color_default(ray));
}

static void					system_render_handle(t_ecs_world *world,
							uint64_t entity_index)
{
	t_app			*app;
	t_ray			*rays;
	t_pixel			*pixel;
	t_scene			*data;
	int				i;
	uint32_t		colors[((t_scene*)(
		(t_app*)ecs_world_system_get(world, system_render).params
	)->data)->ray_samples];

	app = (t_app*)ecs_world_system_get(world, system_render).params;
	data = (t_scene*)app->data;
	rays = (t_ray*)ecs_world_entity_component_get(world,
		entity_index, comp_ray);
	pixel = (t_pixel*)ecs_world_entity_component_get(world,
		entity_index, comp_pixel);
	i = -1;
	while (++i < data->ray_samples)
		colors[i] = color(data->object_tree->root, &rays[i], 0);
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
