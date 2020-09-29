/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   demo_entities.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 19:22:57 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/29 21:30:39 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "demo_raycast.h"

void						entity_ray_update(t_app *app,
							t_ray *ray, int x, int y)
{
	float		scale;
	float		aspect_ratio;
	t_scene		*data;

	data = (t_scene*)app->data;
	scale = data->scale;
	aspect_ratio = data->aspect_ratio;
	ray->dir[0] = (2 * (x + l3d_rand_d() + 0.5) /
		(float)app->window->width - 1) * aspect_ratio * scale;
	ray->dir[1] = (1 - 2 * (y + l3d_rand_d() + 0.5) /
		(float)app->window->height) * scale;
	ray->dir[2] = -3.0;
	ml_vector3_sub(ray->dir, data->camera_pos, ray->dir);
	l3d_ray_set(ray->dir, data->camera_pos, ray);
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
