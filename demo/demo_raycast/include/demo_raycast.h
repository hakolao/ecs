/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   demo_raycast.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 17:46:27 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/23 22:43:24 by ohakola          ###   ########.fr       */
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

# define OBJ_PATH "demo/assets/icosphere.obj"

typedef struct				s_demo_data
{
	t_3d_object				*object;
	t_vec3					camera_pos;
	float					fov;
}							t_demo_data;

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

#endif
