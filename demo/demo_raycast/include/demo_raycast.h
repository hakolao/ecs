/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   demo_raycast.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 17:46:27 by ohakola           #+#    #+#             */
/*   Updated: 2020/10/02 20:18:19 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEMO_RAYCAST_H
# define DEMO_RAYCAST_H

# include "demo_common.h"
# include "libgmatrix.h"
# include "lib3d.h"

# define NUM_THREADS 8

# define NAME "Demo Raycast"

# define OBJ_PATH "demo/assets/f16.obj"
// # define OBJ_PATH "demo/assets/random.obj"
// # define OBJ_PATH "demo/assets/icosphere.obj"

# define RAY_SAMPLES 4
# define MAX_BOUNCES 4

typedef struct				s_scene
{
	t_3d_object				**objects;
	uint32_t				num_objects;
	t_kd_tree				*object_tree;
	t_triangle				*triangle_ref[L3D_MAX_TRIANGLES];
	uint32_t				num_triangles;
	t_vec3					camera_pos;
	float					fov;
	float					scale;
	float					aspect_ratio;
	t_vec3					player_pos;
	t_vec3					player_up;
	t_vec3					player_forward;
	t_vec3					player_sideways;
	float					player_speed;
	float					player_rot_speed;
	t_mat4					world_rotation;
	t_mat4					world_translation;
	t_mat4					world_scale;
	int						ray_samples;
}							t_scene;

typedef enum				e_move
{
	move_forward,
	move_backward,
	move_strafe_left,
	move_strafe_right
}							t_move;

/*
** Component identifiers, should be powers of 2 and ULL for valid component
** ids.
*/

typedef enum				e_comp_id
{
	comp_empty = 0ULL,
	comp_ray = 1ULL,
	comp_pixel = 1ULL << 1,
}							t_comp_id;

/*
** System ids.
** If a system should not be run with other systems, its id should not be 0
*/

typedef enum				e_system_id
{
	system_render = 111,
}							t_system_id;

/*
** System
*/

void						systems_create(t_app *app);
void						systems_params_update(t_app *app);

/*
** Entities
*/

void						entity_rays_create(t_app *app);
void						entity_ray_update(t_app *app,
							t_ray *ray, int x, int y);

/*
** Scene
*/

void						demo_scene_create(t_app *app);
void						demo_scene_destroy(t_app *app);
void						update_world_rotation(t_scene *scene, t_mat4 new_rotation);
void						update_world_translation(t_scene *scene, t_mat4	translation);
void						update_world_scale(t_scene *scene, t_mat4 new_scale);
void						update_scene_triangle_tree(t_scene *scene);

/*
** 3d Object functions
*/

void						l3d_3d_object_destroy(t_3d_object *object);
void						obj_content_to_scene(t_scene *scene, t_obj_content *obj);
void						read_objects_to_scene(t_scene *scene, const char *filename);

/*
** Player
*/
void						move_player(t_scene *data,
							uint32_t delta_time, t_move dir);
void						rotate_player(t_scene *data,
							uint32_t delta_time, t_vec3 axes);

/*
** Math utils
*/

double						l3d_rand_d();

#endif
