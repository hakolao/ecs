/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   demo_entities.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 19:22:57 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/29 00:01:42 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "demo_raycast.h"

void						set_ray(t_vec3 dir, t_vec3 origin, t_ray *ray)
{
	ml_vector3_set(ray->dir, dir[0], dir[1], dir[2]);
	ml_vector3_set(ray->origin, origin[0], origin[1], origin[2]);
	ml_vector3_normalize(ray->dir, ray->dir);
	ml_vector3_set(ray->dir_inv, 1.0 / dir[0], 1.0 / dir[1], 1.0 / dir[2]);
}

void						entity_ray_update(t_app *app,
							t_ray *ray, int x, int y)
{
	float		scale;
	float		aspect_ratio;
	t_scene		*data;

	data = (t_scene*)app->data;
	scale = data->scale;
	aspect_ratio = data->aspect_ratio;
	ray->dir[0] = (2 * (x + rand_d() + 0.5) /
		(float)app->window->width - 1) * aspect_ratio * scale;
	ray->dir[1] = (1 - 2 * (y + rand_d() + 0.5) /
		(float)app->window->height) * scale;
	ray->dir[2] = -3.0;
	ml_vector3_sub(ray->dir, data->camera_pos, ray->dir);
	set_ray(ray->dir, data->camera_pos, ray);
}

void						entity_rays_create(t_app *app)
{
	int		x;
	int		y;
	int		i;
	t_ray	ray[((t_scene*)app->data)->ray_samples];

	y = -1;
	while (++y < app->window->height)
	{
		x = -1;
		while (++x < app->window->width)
		{
			i = -1;
			while (++i < ((t_scene*)app->data)->ray_samples)
				entity_ray_update(app, &ray[i], x, y);
			ecs_world_entity_add(app->world, 2,
				&(t_component){.data = &ray, .id = comp_ray,
					.size = sizeof(ray)},
				&(t_component){.data = &(t_pixel){.x = x, .y = y},
					.id = comp_pixel, .size = sizeof(t_pixel)});
		}
	}
}
