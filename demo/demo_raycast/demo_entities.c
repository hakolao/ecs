/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   demo_entities.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 19:22:57 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/23 22:20:25 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "demo_raycast.h"

void						entity_rays_create(t_app *app)
{
	float	x_pos;
	float	y_pos;
	int		x;
	int		y;
	t_ray	ray;

	y = -1;
	while (++y < app->window->height)
	{
		x = -1;
		while (++x < app->window->width)
		{
			x_pos = (float)x / (float)app->window->width;
			y_pos = (float)y / (float)app->window->height;
			ray.dir[0] = 0;
			ray.dir[1] = 0;
			ray.dir[2] = 1;
			ray.origin[0] = 0;
			ray.origin[1] = 0;
			ray.origin[2] = 0;
			ecs_world_entity_add(app->world, 1, &(t_component){
				.data = &ray, .id = comp_ray, .size = sizeof(t_ray)});
		}
	}
}
