/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   demo_system.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 19:20:36 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/17 23:22:39 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "demo.h"

static void					system_movement_handle(t_ecs_world *world,
							uint64_t entity)
{
	void			*system_args;
	t_position		*pos;
	t_velocity		*vel;
	uint64_t		dt;

	if (!ecs_world_entity_contains(entity, comp_vel | comp_pos))
		return ;
	system_args = world->systems[ecs_system_index(world, system_move)].params;
	if (system_args != NULL)
	{
		dt = *(uint64_t*)&system_args;
		pos = (t_position*)hash_map_get(ecs_component_entities(world, comp_pos),
			entity);
		vel = (t_velocity*)hash_map_get(ecs_component_entities(world, comp_vel),
			entity);
		if (pos && vel)
		{
			pos->x += vel->dx * dt;
			pos->y += vel->dy * dt;
		}
	}
}

static void					system_render_handle(t_ecs_world *world,
							uint64_t entity)
{
	// t_hash_table	*velocities;
	// t_hash_table	*positions;
	// t_hash_table	*render_specs;

	// if (!ecs_world_entity_contains(entity, comp_vel | comp_pos))
	// 	return ;
	// velocities = world->component_list[ecs_component_index(world, comp_vel)];
	// positions = world->component_list[ecs_component_index(world, comp_pos)];
	// render_specs =
	// 	world->component_list[ecs_component_index(world, comp_render)];
	(void)world;
	(void)entity;
}

/*
** Define systems & their param initial values
*/

void						systems_create(t_app *app)
{
	ecs_world_system_add(app->world, (t_system){
		.system_id = system_render,
		.components_mask = comp_render,
		.system_handle_func = system_render_handle,
		.params = NULL
	});
	ecs_world_system_add(app->world, (t_system){
		.system_id = system_move,
		.components_mask = comp_pos | comp_vel,
		.system_handle_func = system_movement_handle,
		.params = app->delta_time
	});
}

/*
** Update system params, e.g. delta_time for velocity calculations
*/

void						systems_params_update(t_app *app)
{
	ecs_system_update_params(app->world, comp_render,
		app->delta_time, sizeof(app->delta_time));
}
