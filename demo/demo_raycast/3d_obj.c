/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3d_obj.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/23 11:35:05 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/23 11:43:03 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "demo_raycast.h"

static void		init_3d_object(t_obj_result *read_obj, t_3d_object *obj)
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
			ml_vector3_copy(read_obj->v[v_i],
				obj->mesh_vertices[v_i].pos);
			obj->mesh_vertices[v_i].color = 0xFFFFFFFF;
		}
		obj->mesh_triangles[i].vtc[0] =
			&obj->mesh_vertices[read_obj->triangles[i][0][0] - 1];
		obj->mesh_triangles[i].vtc[1] =
			&obj->mesh_vertices[read_obj->triangles[i][1][0] - 1];
		obj->mesh_triangles[i].vtc[2] =
			&obj->mesh_vertices[read_obj->triangles[i][2][0] - 1];
	}
}

t_3d_object		*create_3d_object(t_obj_result *read_obj)
{
	t_3d_object	*obj;

	error_check(!(obj = malloc(sizeof(*obj))), "Failed to malloc 3d obj");
	error_check(!(obj->mesh_vertices =
		malloc(sizeof(t_vertex) * read_obj->num_vertices)),
		"Failed to malloc 3d obj vertices");
	error_check(!(obj->mesh_triangles =
		malloc(sizeof(t_triangle) * read_obj->num_triangles)),
		"Failed to malloc 3d obj triangles");
	init_3d_object(read_obj, obj);
	obj->mesh_triangle_count = read_obj->num_triangles;
	obj->mesh_vertex_count = read_obj->num_vertices;
	return (obj);
}

void			destroy_object(t_3d_object *object)
{
	free(object->mesh_triangles);
	free(object->mesh_vertices);
	free(object);
	object = NULL;
}
