/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   demo_entities.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 19:22:57 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/24 12:31:39 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "demo_raycast.h"

void						entity_rays_create(t_app *app)
{
	float	scale;
	float	aspect_ratio;
	int		x;
	int		y;
	t_ray	ray;

	scale = tan(ml_rad(((t_demo_data*)app->data)->fov * 0.5));
 	aspect_ratio = app->window->width / (float)app->window->height;
	y = -1;
	while (++y < app->window->height)
	{
		x = -1;
		while (++x < app->window->width)
		{
			ray.dir[0] = (2 * (x + 0.5) / (float)app->window->width - 1)
				* aspect_ratio * scale;
			ray.dir[1] = (1 - 2 * (y + 0.5) / (float)app->window->height) *
				scale;
			ray.dir[2] = -1.0;
			ml_vector3_copy(((t_demo_data*)(app->data))->camera_pos,
				ray.origin);
			ml_vector3_sub(ray.dir, ray.origin, ray.dir);
			ml_vector3_normalize(ray.dir, ray.dir);
			ecs_world_entity_add(app->world, 2,
				&(t_component){.data = &ray, .id = comp_ray,
					.size = sizeof(t_ray)},
				&(t_component){.data = &(t_pixel){.x = x, .y = y},
					.id = comp_pixel, .size = sizeof(t_ray)});
		}
	}
}
