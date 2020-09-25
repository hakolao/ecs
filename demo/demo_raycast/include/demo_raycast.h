/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   demo_raycast.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 17:46:27 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/25 17:41:02 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEMO_RAYCAST_H
# define DEMO_RAYCAST_H

# include "demo_common.h"
# include "libgmatrix.h"
# include "kd_tree.h"

# define NUM_THREADS 8

# define NAME "Demo Raycast"

# define NUM_TRIANGLES 5

# define MAX_OBJ_TRIANGLES 1024

# define MAX_OBJECTS 10

# define ICOSPHERE_PATH "demo/assets/icosphere.obj"

typedef struct				s_demo_data
{
	t_3d_object				*objects[MAX_OBJECTS];
	uint32_t				num_objects;
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
}							t_demo_data;

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

/*
** Player
*/
void						move_player(t_demo_data *data,
							uint32_t delta_time, t_move dir);
void						rotate_player(t_demo_data *data,
							uint32_t delta_time, t_vec3 axes);

#endif
