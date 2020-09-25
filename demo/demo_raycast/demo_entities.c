/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   demo_entities.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 19:22:57 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/25 16:07:30 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "demo_raycast.h"

void						entity_ray_update(t_app *app,
							t_ray *ray, int x, int y)
{
	float		scale;
	float		aspect_ratio;
	t_demo_data	*data;

	data = (t_demo_data*)app->data;
	scale = data->scale;
	aspect_ratio = data->aspect_ratio;
	ray->dir[0] = (2 * (x + 0.5) /
		(float)app->window->width - 1) * aspect_ratio * scale;
	ray->dir[1] = (1 - 2 * (y + 0.5) /
		(float)app->window->height) * scale;
	ray->dir[2] = -3.0;
	ml_vector3_copy(data->player_pos, ray->origin);
	ml_vector3_sub(ray->dir, ray->origin, ray->dir);
	ml_vector3_normalize(ray->dir, ray->dir);
}

void						entity_rays_create(t_app *app)
{
	int		x;
	int		y;
	t_ray	ray;

	y = -1;
	while (++y < app->window->height)
	{
		x = -1;
		while (++x < app->window->width)
		{
			entity_ray_update(app, &ray, x, y);
			ecs_world_entity_add(app->world, 2,
				&(t_component){.data = &ray, .id = comp_ray,
					.size = sizeof(t_ray)},
				&(t_component){.data = &(t_pixel){.x = x, .y = y},
					.id = comp_pixel, .size = sizeof(t_ray)});
		}
	}
}
