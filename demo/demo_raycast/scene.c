/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/24 12:06:53 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/25 18:27:16 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "demo_raycast.h"

void			apply_transforms_to_world(t_demo_data *data)
{
	int		i;

	i = -1;
	while (++i < (int)data->num_objects)
	{
		transform_3d_object(data->objects[i], data->world_scale, trans_scale);
		transform_3d_object(data->objects[i], data->world_rotation, trans_rotate);
		transform_3d_object(data->objects[i], data->world_translation, trans_translate);
		update_3d_object_kd_tree(data->objects[i]);
	}
}

static void		init_scene_player(t_demo_data *data)
{
	ft_memcpy(&data->player_pos, &(t_vec3){0, 0, 0}, sizeof(t_vec3));
	ft_memcpy(&data->player_forward, &(t_vec3){0, 0, 1}, sizeof(t_vec3));
	ft_memcpy(&data->player_up, &(t_vec3){0, 1, 0}, sizeof(t_vec3));
	ft_memcpy(&data->player_sideways, &(t_vec3){1, 0, 0}, sizeof(t_vec3));
	data->player_speed = 0.5f;
	data->player_rot_speed = 0.1f;
}

/*
** ToDo: Make all objects one kd_tree instead of tree per object...
*/

void			demo_scene_create(t_app *app)
{
	t_demo_data *data;
	uint32_t	i;

	data = (t_demo_data*)app->data;
	init_scene_player(data);
	i = -1;
	while (++i < MAX_OBJECTS)
		data->objects[i] = NULL;
	data->objects[0] = read_object_file(ICOSPHERE_PATH);
	data->objects[1] = read_object_file(ICOSPHERE_PATH);
	data->num_objects = 1;
	ml_vector3_copy((t_vec3){0, 0, 0}, data->camera_pos);
	data->fov = 90.0;
	ml_matrix4_id(data->world_scale);
	ml_matrix4_id(data->world_rotation);
	data->world_scale[0][0] = 30;
	data->world_scale[1][1] = 30;
	data->world_scale[2][2] = 30;
	ml_matrix4_translation(0, 0, -200, data->world_translation);
	apply_transforms_to_world(data);
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
}
