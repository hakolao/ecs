/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/24 12:06:53 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/25 14:39:05 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "demo_raycast.h"

static void		init_scene_player(t_demo_data *data)
{
	ft_memcpy(&data->player_pos, &(t_vec3){0, 0, 0}, sizeof(t_vec3));
	ft_memcpy(&data->player_forward, &(t_vec3){0, 0, 1}, sizeof(t_vec3));
	ft_memcpy(&data->player_up, &(t_vec3){0, 1, 0}, sizeof(t_vec3));
	data->player_speed = 0.5f;
	data->player_rot_speed = 0.1f;
}

/*
** ToDo: Make all objects one kd_tree instead of tree per object...
*/

void			demo_scene_create(t_app *app)
{
	t_demo_data *data;
	t_mat4		translate;
	t_mat4		scale;
	t_mat4		transform;
	uint32_t	i;

	data = (t_demo_data*)app->data;
	init_scene_player(data);
	i = -1;
	while (++i < MAX_OBJECTS)
		data->objects[i] = NULL;
	data->objects[0] = read_object_file(ICOSPHERE_PATH);
	data->objects[1] = read_object_file(ICOSPHERE_PATH);
	data->num_objects = 2;
	ml_vector3_copy((t_vec3){0, 0, 0}, data->camera_pos);
	data->fov = 100.0;
	ml_matrix4_id(scale);
	scale[0][0] = 30;
	scale[1][1] = 30;
	scale[2][2] = 30;
	ml_matrix4_translation(0, 0, -100, translate);
	ml_matrix4_mul(translate, scale, transform);
	transform_3d_object(data->objects[0], transform);
	ml_matrix4_translation(-50, 0, -100, translate);
	ml_matrix4_mul(translate, scale, transform);
	transform_3d_object(data->objects[1], transform);
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
