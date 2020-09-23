/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   demo_raycast.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 17:46:27 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/23 11:43:57 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEMO_RAYCAST_H
# define DEMO_RAYCAST_H

# include "demo_common.h"
# include "libgmatrix.h"

# define NUM_THREADS 8

# define NAME "Demo Raycast"

# define NUM_TRIANGLES 5

# define MAX_OBJ_TRIANGLES 1024

# define OBJ_PATH "demo/assets/icosphere.obj"

typedef struct				s_vertex
{
	t_vec4					pos;
	uint32_t				color;
}							t_vertex;

typedef struct				s_triangle
{
	t_vertex				*vtc[3];
}							t_triangle;

typedef struct				s_box3d
{
	t_vec3					top[4];
	t_vec3					bottom[4];
}							t_box3d;

typedef struct				s_3d_object
{
	t_vertex				*mesh_vertices;
	int32_t					mesh_vertex_count;
	t_triangle				*mesh_triangles;
	int32_t					mesh_triangle_count;
	t_box3d					mesh_bound_box;
}							t_3d_object;

typedef struct				s_obj_result
{
	t_vec3			v[MAX_OBJ_TRIANGLES];
	uint32_t		num_vertices;
	t_vec2			vt[MAX_OBJ_TRIANGLES];
	uint32_t		num_v_text_coords;
	t_vec3			vn[MAX_OBJ_TRIANGLES];
	uint32_t		num_v_normals;
	uint32_t		triangles[MAX_OBJ_TRIANGLES][3][3];
	uint32_t		num_triangles;
}							t_obj_result;

typedef struct				s_demo_data
{
	uint32_t				num_triangles;
}							t_demo_data;

/*
** Component identifiers, should be powers of 2 and ULL for valid component
** ids.
*/

typedef enum				e_comp_id
{
	comp_empty = 0ULL,
	comp_triangle = 1ULL,
	comp_ray = 1ULL << 1,
}							t_comp_id;

/*
** System ids.
** If a system should not be run with other systems, its id should not be 0
*/

typedef enum				e_system_id
{
	system_render = 111,
	system_zbuffer = 222,
}							t_system_id;

/*
** System
*/
void						systems_create(t_app *app);
void						systems_params_update(t_app *app);

/*
** Entities
*/
//ToDo

/*
** 3d object
*/
void						destroy_object(t_3d_object *object);
t_3d_object					*create_3d_object(t_obj_result *read_obj);

#endif
