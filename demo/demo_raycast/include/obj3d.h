/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 21:49:59 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/23 18:06:55 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJ3D_H
# define OBJ3D_H

# include <stdlib.h>
# include <float.h>
# include "t_bool.h"
# include "libgmatrix.h"

# define MAX_TRIANGLES 1024
# define TRI_VEC_INITIAL_CAPACITY 10

typedef struct				s_vertex
{
	t_vec4					pos;
	uint32_t				color;
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
}							t_triangle;

/*
** Struct to hold .obj file data (which is transformed into a 3d object)
*/

typedef struct				s_obj_result
{
	t_vec3			v[MAX_TRIANGLES];
	uint32_t		num_vertices;
	t_vec2			vt[MAX_TRIANGLES];
	uint32_t		num_v_text_coords;
	t_vec3			vn[MAX_TRIANGLES];
	uint32_t		num_v_normals;
	uint32_t		triangles[MAX_TRIANGLES][3][3];
	uint32_t		num_triangles;
}							t_obj_result;

typedef struct				s_tri_vec
{
	t_triangle	**triangles;
	uint32_t	size;
	uint32_t	capacity;
}							t_tri_vec;

/*
** Kd Tree
*/

typedef struct	s_kd_node	t_kd_node;

struct			s_kd_node
{
	uint32_t	uuid;
	t_box3d		bounding_box;
	uint32_t	axis;
	t_tri_vec	*triangles;
	t_kd_node	*left;
	t_kd_node	*right;
};

typedef struct				s_kd_tree
{
	uint32_t	num_nodes;
	t_kd_node	*root;
}							t_kd_tree;

/*
** 3d object
*/

typedef struct				s_3d_object
{
	t_vertex				**vertices;
	int32_t					num_vertices;
	t_triangle				*triangles;
	int32_t					num_triangles;
	t_kd_tree				*triangle_tree;
}							t_3d_object;


/*
** 3d Object functions
*/

void						destroy_object(t_3d_object *object);
t_3d_object					*create_3d_object(t_obj_result *read_obj);
t_3d_object					*read_object_file(const char *filename);


/*
** Kd Tree functions
*/

t_kd_tree					*kd_tree_create(t_triangle *triangles,
							uint32_t num_triangles);
void						kd_tree_destroy(t_kd_tree *tree);
void						kd_tree_print(t_kd_node *root);
void						kd_tree_split_triangles(t_tri_vec *triangles,
							uint32_t axis, t_tri_vec *left_tris,
							t_tri_vec *right_tris);
void						get_kd_mid_point(t_tri_vec *triangles, t_vec3 res);
int							get_bounding_box_longest_axis(t_box3d bounding_box);
void						get_bounding_box(t_tri_vec *triangles, t_box3d *res);

/*
** Triangle vector
*/

void						triangle_vec_push(t_tri_vec *vector,
							t_triangle *triangle);
t_tri_vec					*triangle_vec_empty(void);
t_tri_vec					*triangle_vec(t_triangle *triangles,
							uint32_t num_triangles);
void						tri_vec_delete(t_tri_vec *vector);

#endif
