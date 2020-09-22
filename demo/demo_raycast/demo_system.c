/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   demo_system.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 19:20:36 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/22 17:24:27 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "demo_raycast.h"

static void					system_zbuffer_handle(t_ecs_world *world,
							uint64_t entity_index)
{
	(void)world;
	(void)entity_index;
}

static void					system_render_handle(t_ecs_world *world,
							uint64_t entity_index)
{
	(void)world;
	(void)entity_index;
}

/*
** Define systems & their param initial values
*/

void						systems_create(t_app *app)
{
	ecs_world_system_add(app->world, (t_system){
		.system_id = system_zbuffer,
		.components_mask = comp_ray,
		.system_handle_func = system_zbuffer_handle,
		.params = app
	});
	ecs_world_system_add(app->world, (t_system){
		.system_id = system_render,
		.components_mask = comp_ray,
		.system_handle_func = system_render_handle,
		.params = app
	});
}

void						systems_params_update(t_app *app)
{
	ecs_system_update_params(app->world, system_zbuffer, app);
	ecs_system_update_params(app->world, system_render, app);
}
