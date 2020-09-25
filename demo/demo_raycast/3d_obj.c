/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3d_obj.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/23 11:35:05 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/25 15:00:35 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "demo_raycast.h"

static void		calculate_triangle_centroid(t_triangle *triangle)
{
		ml_vector3_copy((t_vec3){
			(triangle->vtc[0]->pos[0] + triangle->vtc[1]->pos[0] +
			triangle->vtc[2]->pos[0]) / 3.0,
			(triangle->vtc[0]->pos[1] + triangle->vtc[1]->pos[1] +
			triangle->vtc[2]->pos[1]) / 3.0,
			(triangle->vtc[0]->pos[2] + triangle->vtc[1]->pos[2] +
			triangle->vtc[2]->pos[2]) / 3.0,
		}, triangle->center);
}

static void		obj_file_to_3d_obj(t_obj_result *read_obj, t_3d_object *obj)
{
	int		i;
	int		j;
	int		v_i;
	// int		vt_i;
	// int		vn_i;

	i = -1;
	while (++i < (int)read_obj->num_triangles)
	{
		j = -1;
		while (++j < 3)
		{
			v_i = read_obj->triangles[i][j][0] - 1;
			// vt_i = read_obj->triangles[i][j][1] - 1;
			// vn_i = read_obj->triangles[i][j][2] - 1;
			if (obj->vertices[v_i] == NULL)
				error_check(!(obj->vertices[v_i] =
					malloc(sizeof(t_vertex))), "Failed to malloc vertex");
			ml_vector3_copy(read_obj->v[v_i],
				obj->vertices[v_i]->pos);
			obj->vertices[v_i]->color = 0xFFFFFFFF;
		}
		obj->triangles[i].vtc[0] =
			obj->vertices[read_obj->triangles[i][0][0] - 1];
		obj->triangles[i].vtc[1] =
			obj->vertices[read_obj->triangles[i][1][0] - 1];
		obj->triangles[i].vtc[2] =
			obj->vertices[read_obj->triangles[i][2][0] - 1];
		calculate_triangle_centroid(&obj->triangles[i]);
	}
}

t_3d_object		*create_3d_object(t_obj_result *read_obj)
{
	t_3d_object	*obj;

	error_check(!(obj = malloc(sizeof(*obj))), "Failed to malloc 3d obj");
	error_check(!(obj->vertices =
		malloc(sizeof(t_vertex*) * read_obj->num_vertices)),
		"Failed to malloc 3d obj vertices");
	ft_memset(obj->vertices, 0,
		sizeof(t_vertex*) * read_obj->num_vertices);
	error_check(!(obj->triangles =
		malloc(sizeof(t_triangle) * read_obj->num_triangles)),
		"Failed to malloc 3d obj triangles");
	obj_file_to_3d_obj(read_obj, obj);
	obj->num_triangles = read_obj->num_triangles;
	obj->num_vertices = read_obj->num_vertices;
	ml_matrix4_id(obj->transform);
	obj->triangle_tree = kd_tree_create(obj->triangles, obj->num_triangles);
	return (obj);
}

void			transform_3d_object(t_3d_object *obj, t_mat4 transform)
{
	int		i;
	t_mat4	new_transform;

	ml_matrix4_mul(obj->transform, transform, new_transform);
	ft_memcpy(obj->transform, new_transform, sizeof(t_mat4));
	i = -1;
	while (++i < obj->num_vertices)
		ml_matrix4_mul_vec3(transform,
			obj->vertices[i]->pos, obj->vertices[i]->pos);
	if (obj->triangle_tree != NULL)
		kd_tree_destroy(obj->triangle_tree);
	obj->triangle_tree = kd_tree_create(obj->triangles, obj->num_triangles);
}

void			destroy_object(t_3d_object *object)
{
	kd_tree_destroy(object->triangle_tree);
	free(object->triangles);
	free(object->vertices);
	free(object);
	object = NULL;
}
