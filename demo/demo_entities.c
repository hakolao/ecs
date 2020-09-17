/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   demo_entities.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 19:22:57 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/17 20:11:23 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "demo.h"

void						entities_create(t_app *app)
{
	uint32_t	i;
	uint32_t	interval;
	uint32_t	size;

	i = 1;
	interval = WIDTH / MAX_ENTITIES;
	while (app->world->num_entities < MAX_ENTITIES)
	{
		size = ft_rand(i * 3 + 2) % 40;
		ecs_world_entity_add(app->world, 3,
			&(t_component){.id = comp_pos, .size = sizeof(t_position),
				.data = &(t_position){
					.x = i,
					.y = (float)(ft_rand(i) % 30)}},
			&(t_component){.id = comp_vel, .size = sizeof(t_velocity),
				.data = &(t_velocity){
					.dx = 0.1 * (float)(ft_rand(i % 2 == 0 ? -i : i) % 10),
					.dy = (float)(ft_rand(i * 2 + 1) % 20)}},
			&(t_component){.id = comp_render, .size = sizeof(t_render),
				.data = &(t_render){
					.color = (1 << 31) / i,
					.width = size,
					.height = size,
					.window = &app->window}});
		i++;
		if (i >= WIDTH - 1)
			i = 1;
	}
}
