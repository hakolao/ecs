/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   demo_system.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 19:20:36 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/17 19:50:15 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "demo.h"

static void					system_movement_handle(t_ecs_world *world)
{
	(void)world;
}

static void					system_render_handle(t_ecs_world *world)
{
	(void)world;
}

void						systems_create(t_ecs_world *world)
{
	ecs_world_system_add(world, (t_system){
		.system_id = system_render,
		.components_mask = comp_specs,
		.system_handle_func = system_render_handle
	});
	ecs_world_system_add(world, (t_system){
		.system_id = system_move,
		.components_mask = comp_pos | comp_vel,
		.system_handle_func = system_movement_handle
	});
}
