/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/24 12:06:53 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/24 12:16:40 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "demo_raycast.h"

void		demo_scene_create(t_app *app)
{
	t_demo_data *data;
	uint32_t	i;

	data = (t_demo_data*)app->data;
	i = -1;
	while (++i < MAX_OBJECTS)
		data->scene[i] = NULL;
	data->scene[0] = read_object_file(ICOSPHERE_PATH);
	data->num_objects = 1;
	ml_vector3_copy((t_vec3){0, 0, 0}, data->camera_pos);
	data->fov = 45.0;
}

void		demo_scene_destroy(t_app *app)
{
	t_demo_data *data;
	uint32_t	i;

	data = (t_demo_data*)app->data;
	i = -1;
	while (++i < data->num_objects)
	{
		destroy_object(data->scene[i]);
		data->scene[i] = NULL;
	}
}
