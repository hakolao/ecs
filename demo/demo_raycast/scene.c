/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/24 12:06:53 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/29 21:39:18 by ohakola          ###   ########.fr       */
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

static void		demo_scene_set_transforms(t_scene *scene)
{
	ml_matrix4_id(scene->world_scale);
	ml_matrix4_rotation_x(ml_rad(25.0), scene->world_rotation);
	update_world_rotation(scene, scene->world_rotation);
	ml_matrix4_id(scene->world_translation);
	ml_matrix4_id(scene->world_scale);
	scene->world_scale[0][0] = 15;
	scene->world_scale[1][1] = 15;
	scene->world_scale[2][2] = 15;
	update_world_scale(scene, scene->world_scale);
	ml_matrix4_translation(0, -30.0, -350, scene->world_translation);
	update_world_translation(scene, scene->world_translation);
}

static void		demo_scene_set_triangle_refs(t_scene *scene)
{
	int		i;
	int		j;
	int		k;
	int		num_triangles;

	i = -1;
	num_triangles = 0;
	k = 0;
	while (++i < (int)scene->num_objects)
	{
		j = -1;
		while (++j < scene->objects[i]->num_triangles)
		{
			scene->triangle_ref[k] = &scene->objects[i]->triangles[j];
			num_triangles++;
			k++;
		}
	}
	scene->num_triangles = num_triangles;
}

/*
** ToDo: Make all objects one kd_tree instead of tree per object...
*/

void			demo_scene_create(t_app *app)
{
	t_scene *scene;

	scene = (t_scene*)app->data;
	init_scene_player(scene);
	scene->objects = l3d_read_obj(OBJ_PATH, &scene->num_objects);
	ml_vector3_copy((t_vec3){0, 0, 0}, scene->camera_pos);
	scene->fov = 90.0;
	demo_scene_set_transforms(scene);
	demo_scene_set_triangle_refs(scene);
	l3d_kd_tree_create_or_update(&scene->object_tree, scene->triangle_ref,
		scene->num_triangles);
}

void			demo_scene_destroy(t_app *app)
{
	t_scene *scene;
	uint32_t	i;

	scene = (t_scene*)app->data;
	i = -1;
	l3d_kd_tree_destroy(scene->object_tree);
	while (++i < scene->num_objects)
	{
		l3d_3d_object_destroy(scene->objects[i]);
		scene->objects[i] = NULL;
	}
	free(scene->objects);
}
