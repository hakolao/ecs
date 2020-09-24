/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/24 12:06:53 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/24 13:05:48 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "demo_raycast.h"

void		demo_scene_create(t_app *app)
{
	t_demo_data *data;
	t_mat4		translate;
	t_mat4		scale;
	t_mat4		transform;
	uint32_t	i;

	data = (t_demo_data*)app->data;
	i = -1;
	while (++i < MAX_OBJECTS)
		data->objects[i] = NULL;
	data->objects[0] = read_object_file(ICOSPHERE_PATH);
	data->num_objects = 1;
	ml_vector3_copy((t_vec3){0, 0, 0}, data->camera_pos);
	data->fov = 45.0;
	ml_matrix4_id(scale);
	scale[0][0] = 15;
	scale[1][1] = 15;
	scale[2][2] = 15;
	ml_matrix4_translation(0, 0, -100, translate);
	ml_matrix4_mul(translate, scale, transform);
	transform_3d_object(data->objects[0], transform);
	kd_tree_print(data->objects[0]->triangle_tree->root);
}

void		demo_scene_destroy(t_app *app)
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
