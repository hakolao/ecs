/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/24 12:06:53 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/27 22:25:05 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "demo_raycast.h"

static void		init_scene_player(t_scene *scene)
{
	ft_memcpy(&scene->player_pos, &(t_vec3){0, 0, 0}, sizeof(t_vec3));
	ft_memcpy(&scene->player_forward, &(t_vec3){0, 0, 1}, sizeof(t_vec3));
	ft_memcpy(&scene->player_up, &(t_vec3){0, 1, 0}, sizeof(t_vec3));
	ft_memcpy(&scene->player_sideways, &(t_vec3){1, 0, 0}, sizeof(t_vec3));
	scene->player_speed = 0.5f;
	scene->player_rot_speed = 0.1f;
}

static void		init_world_transform(t_scene *scene)
{
	ml_matrix4_id(scene->world_transform);
	ml_matrix4_id(scene->world_scale);
	ml_matrix4_id(scene->world_rotation);
	ml_matrix4_translation(0, 0, 2, scene->world_translation);
}

/*
** ToDo: Make all objects one kd_tree instead of tree per object...
*/

void			demo_scene_create(t_app *app)
{
	t_scene *scene;

	scene = (t_scene*)app->data;
	init_scene_player(scene);
	read_objects_to_scene(scene, ICOSPHERE_PATH);
	scene->num_objects = 1;
	ml_vector3_copy((t_vec3){0, 0, 0}, scene->camera_pos);
	scene->fov = 90.0;
	init_world_transform(scene);
	update_world_transform(scene);
}

void			demo_scene_destroy(t_app *app)
{
	t_scene *scene;
	uint32_t	i;

	scene = (t_scene*)app->data;
	i = -1;
	kd_tree_destroy(scene->object_tree);
	while (++i < scene->num_objects)
	{
		destroy_object(scene->objects[i]);
		scene->objects[i] = NULL;
	}
	free(scene->objects);
}

void			update_scene_triangle_tree(t_scene *scene)
{
	if (scene->object_tree != NULL)
		kd_tree_destroy(scene->object_tree);
	scene->object_tree =
		kd_tree_create(scene->triangle_ref, scene->num_triangles);
}
