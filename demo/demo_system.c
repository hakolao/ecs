/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   demo_system.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 19:20:36 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/17 20:47:09 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "demo.h"

static void					entity_render(t_render *params)
{
	(void)params;
	// Render entity based on params (access framebuffer);
}

static void					system_movement_handle(t_ecs_world *world)
{
	uint64_t		system_index;
	uint64_t		component_index;
	t_system		system;
	t_system_handle	render_handle;

	system_index = ecs_system_index(world, comp_render);
	render_handle = world->systems[system_index].system_handle_func;
	component_index = ecs_component_index(world, comp_vel);
	hash_map_foreach(world->component_list[component_index], entity_render);
}

static void					system_render_handle(t_ecs_world *world)
{
	uint64_t		system_index;
	uint64_t		component_index;
	t_system		system;
	t_system_handle	render_handle;

	system_index = ecs_system_index(world, comp_render);
	render_handle = world->systems[system_index].system_handle_func;
	component_index = ecs_component_index(world, comp_render);
	hash_map_foreach(world->component_list[component_index], entity_render);
}

void						systems_create(t_app *app)
{
	ecs_world_system_add(app->world, (t_system){
		.system_id = system_render,
		.components_mask = comp_render,
		.system_handle_func = system_render_handle
	});
	ecs_world_system_add(app->world, (t_system){
		.system_id = system_move,
		.components_mask = comp_pos | comp_vel,
		.system_handle_func = system_movement_handle
	});
}

void						systems_run(t_ecs_world *world)
{

}
