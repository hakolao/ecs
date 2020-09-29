/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 21:49:59 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/29 21:44:16 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIB3D_H
# define LIB3D_H

# include <stdlib.h>
# include <float.h>
# include "t_bool.h"
# include "libgmatrix.h"

# define L3D_EPSILON 0.0000001
# define L3D_SINGLE_SIDED 1
# define L3D_MAX_KD_TREE_DEPTH 10
# define L3D_MIN_KD_NODE_NUM_TRIANGLES 2
# define L3D_TRI_VEC_INITIAL_CAPACITY 10


# define L3D_MAX_OBJECTS 32
# define L3D_MAX_TRIANGLES 16383
# define L3D_MAX_VERTICES 16383

/*
** OBJ file temporary structs
*/

typedef struct				s_obj
{
	t_vec3			*v;
	uint32_t		num_vertices;
	t_vec2			*vt;
	uint32_t		num_v_text_coords;
	t_vec3			*vn;
	uint32_t		num_v_normals;
	uint32_t		*triangles;
	uint32_t		num_triangles;
}							t_obj;

typedef struct				s_obj_content
{
	uint32_t		num_objects;
	t_obj			objects[L3D_MAX_OBJECTS];
}							t_obj_content;

typedef struct				s_ray
{
	t_vec3		dir;
	t_vec3		origin;
	t_vec3		dir_inv;
}							t_ray;

typedef struct				s_hit
{
	float					t;
	t_vec3					normal;
	t_vec3					hit_point;
}							t_hit;

typedef struct				s_vertex
{
	t_vec4					pos;
	uint32_t				color;
	t_vec2					uv;
}							t_vertex;

typedef struct				s_box3d
{
	t_vec3					center;
	t_vec3					size;
	float					xyz_min[3];
	float					xyz_max[3];
}							t_box3d;

typedef struct				s_triangle
{
	t_vertex				*vtc[3];
	t_vec3					center;
	t_vec3					normal;
	t_bool					is_single_sided;
}							t_triangle;

/*
** Final 3d object struct to which obj file is transformed
*/

typedef struct				s_3d_object
{
	t_vertex				**vertices;
	int32_t					num_vertices;
	t_triangle				*triangles;
	int32_t					num_triangles;
	t_vec2					*uvs;
	int32_t					num_uvs;
}							t_3d_object;

typedef enum				e_axis
{
	l3d_axis_x,
	l3d_axis_y,
	l3d_axis_z,
	l3d_axis_none,
}							t_axis;

typedef struct				s_tri_vec
{
	t_triangle	**triangles;
	uint32_t	size;
	uint32_t	capacity;
}							t_tri_vec;

typedef struct	s_kd_node	t_kd_node;
struct			s_kd_node
{
	uint32_t	uuid;
	t_box3d		bounding_box;
	t_axis		axis;
	t_tri_vec	*triangles;
	t_kd_node	*left;
	t_kd_node	*right;
};

typedef struct				s_kd_tree
{
	uint32_t	num_nodes;
	t_kd_node	*root;
}							t_kd_tree;

void						l3d_kd_tree_create_or_update(t_kd_tree **tree,
							t_triangle **triangles, uint32_t num_triangles);
t_kd_tree					*l3d_kd_tree_create(t_triangle **triangles,
							uint32_t num_triangles);
void						l3d_kd_tree_destroy(t_kd_tree *tree);
void						l3d_kd_tree_print(t_kd_tree *tree);

/*
** Ray
*/

t_bool						l3d_kd_tree_ray_hit(t_kd_node *node, t_ray *ray,
							float t_max, t_hit *hit);
t_bool						l3d_triangle_ray_hit(t_triangle *triangle,
							t_ray *ray, t_hit *hit);
t_bool						l3d_bounding_box_ray_hit(t_box3d *box,
							t_ray *ray, t_hit *hit);
void						l3d_ray_set(t_vec3 dir, t_vec3 origin, t_ray *ray);

/*
** Triangle vector utils
*/

void						l3d_triangle_vec_push(t_tri_vec *vector,
							t_triangle *triangle);
t_tri_vec					*l3d_triangle_vec_empty(void);
t_tri_vec					*l3d_triangle_vec(t_triangle **triangles,
							uint32_t num_triangles);
void						l3d_triangle_vec_delete(t_tri_vec *vector);

/*
** Triangles
*/

void						l3d_triangles_midpoint(t_triangle **triangles,
							uint32_t num_triangles, t_vec3 res);
void						l3d_triangle_vec_midpoint(t_tri_vec *triangles, t_vec3 res);
void						l3d_triangle_centroid(t_triangle *triangle);
void						l3d_triangle_normal_update(t_triangle *triangle);

/*
** Bounding box
*/

t_axis						l3d_bounding_box_longest_axis(t_box3d bounding_box);
void						l3d_bounding_box_set(t_tri_vec *triangles,
							t_box3d *res);

/*
** 3d objects
*/

void						l3d_3d_object_transform(t_3d_object *obj, t_mat4 transform);
t_3d_object					*l3d_3d_object_create(uint32_t num_vertices,
							uint32_t num_triangles, uint32_t num_text_coords);
void						l3d_3d_object_destroy(t_3d_object *object);

/*
** OBJ reading
*/

t_3d_object					**l3d_read_obj(const char *filename,
							uint32_t *num_objects);

/*
** Math utils
*/

float						l3d_fmax(float n1, float n2);
float						l3d_fmin(float n1, float n2);
double						l3d_rand_d();

#endif