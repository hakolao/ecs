/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   demo_entities.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 19:22:57 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/18 13:47:25 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "demo.h"

/*
** Create some entities with randomized params :)
*/

void						entities_create(t_app *app)
{
	int32_t		i;
	int32_t		size;

	i = 1;
	while (app->world->num_entities < MAX_ENTITIES)
	{
		size = 3;
		ecs_world_entity_add(app->world, 3,
			&(t_component){.id = comp_pos, .size = sizeof(t_position),
				.data = &(t_position){
					.x = (float)((size / 2) + i % WIDTH) - (float)(size / 2),
					.y = (float)((float)size / 2.0 + (ft_rand(i) % 30)) -
						(float)(size / 2)}},
			&(t_component){.id = comp_vel, .size = sizeof(t_velocity),
				.data = &(t_velocity){
					.dx = 0.01 * (float)(ft_rand(i % 9) % 10),
					.dy = 0.01 * (float)(ft_rand(i * 3 + 1) % 20)}},
			&(t_component){.id = comp_render, .size = sizeof(t_render),
				.data = &(t_render){
					.color = i,
					.width = size,
					.height = size,
					.window = &app->window}});
		i = ft_rand(i);
	}
}
