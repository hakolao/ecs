/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/24 12:06:53 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/27 20:27:06 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "demo_raycast.h"

static void		init_scene_player(t_demo_data *data)
{
	ft_memcpy(&data->player_pos, &(t_vec3){0, 0, 0}, sizeof(t_vec3));
	ft_memcpy(&data->player_forward, &(t_vec3){0, 0, 1}, sizeof(t_vec3));
	ft_memcpy(&data->player_up, &(t_vec3){0, 1, 0}, sizeof(t_vec3));
	ft_memcpy(&data->player_sideways, &(t_vec3){1, 0, 0}, sizeof(t_vec3));
	data->player_speed = 0.5f;
	data->player_rot_speed = 0.1f;
}

static void		init_world_transform(t_demo_data *data)
{
	ml_matrix4_id(data->world_transform);
	ml_matrix4_id(data->world_scale);
	ml_matrix4_id(data->world_rotation);
	ml_matrix4_translation(0, 0, 2, data->world_translation);
}

/*
** ToDo: Make all objects one kd_tree instead of tree per object...
*/

void			demo_scene_create(t_app *app)
{
	t_demo_data *data;

	data = (t_demo_data*)app->data;
	init_scene_player(data);
	data->objects = read_objects_file(ICOSPHERE_PATH);
	data->num_objects = 1;
	ml_vector3_copy((t_vec3){0, 0, 0}, data->camera_pos);
	data->fov = 90.0;
	init_world_transform(data);
	update_world_transform(data);
}

void			demo_scene_destroy(t_app *app)
{
	t_demo_data *data;
	uint32_t	i;

	data = (t_demo_data*)app->data;
	i = -1;
	while (++i < data->num_objects)
	{
		destroy_object(data->objects[i]);
		data->objects[i] = NULL;
	}
	free(data->objects);
}
