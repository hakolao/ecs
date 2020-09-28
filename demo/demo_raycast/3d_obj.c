/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3d_obj.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/23 11:35:05 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/28 16:34:43 by ohakola          ###   ########.fr       */
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

static void		obj_file_to_3d_obj(t_obj *read_obj, t_3d_object *obj)
{
	int		i;
	int		j;
	int		v_i;
	int		vt_i;
	// int		vn_i;

	i = -1;
	while (++i < (int)read_obj->num_triangles)
	{
		j = -1;
		while (++j < 3)
		{
			v_i = read_obj->triangles[i * 9 + j * 3 + 0] - 1;
			vt_i = read_obj->triangles[i * 9 + j * 3 + 1] - 1;
			if (obj->vertices[v_i] == NULL)
				error_check(!(obj->vertices[v_i] =
					malloc(sizeof(t_vertex))), "Failed to malloc vertex");
			ml_vector3_copy(read_obj->v[v_i], obj->vertices[v_i]->pos);
			obj->vertices[v_i]->color = 0xFFFFFFFF;
			ml_vector2_copy(read_obj->vt[vt_i], obj->vertices[v_i]->uv);
		}
		obj->triangles[i].vtc[0] =
			obj->vertices[read_obj->triangles[i * 9 + 0 * 3 + 0] - 1];
		obj->triangles[i].vtc[1] =
			obj->vertices[read_obj->triangles[i * 9 + 1 * 3 + 0] - 1];
		obj->triangles[i].vtc[2] =
			obj->vertices[read_obj->triangles[i * 9 + 2 * 3 + 0] - 1];
		calculate_triangle_centroid(&obj->triangles[i]);
	}
}

void		obj_content_to_scene(t_scene *scene, t_obj_content *obj)
{
	int			num_triangles;
	int			i;
	int			j;

	error_check(!(scene->objects =
		malloc(sizeof(*scene->objects) * obj->num_objects)),
		"Failed to malloc 3d obj");
	i = -1;
	num_triangles = 0;
	while (++i < (int)obj->num_objects)
	{
		error_check(!(scene->objects[i] = malloc(sizeof(*scene->objects[i]))),
			"Failed to malloc 3d obj");
		error_check(!(scene->objects[i]->vertices =
			malloc(sizeof(t_vertex*) * obj->objects[i].num_vertices)),
			"Failed to malloc 3d obj vertices");
		ft_memset(scene->objects[i]->vertices, 0,
			sizeof(t_vertex*) * obj->objects[i].num_vertices);
		error_check(!(scene->objects[i]->triangles =
			malloc(sizeof(t_triangle) * obj->objects[i].num_triangles)),
			"Failed to malloc 3d obj triangles");
		error_check(!(scene->objects[i]->uvs =
			malloc(sizeof(t_vec2) * obj->objects[i].num_v_text_coords)),
			"Failed to malloc 3d obj uvs");
		obj_file_to_3d_obj(&obj->objects[i], scene->objects[i]);
		free(obj->objects[i].v);
		free(obj->objects[i].vt);
		free(obj->objects[i].vn);
		free(obj->objects[i].triangles);
		scene->objects[i]->num_triangles = obj->objects[i].num_triangles;
		scene->objects[i]->num_vertices = obj->objects[i].num_vertices;
		num_triangles -= 1;
		j = 0;
		while (++num_triangles < (int)scene->num_triangles + scene->objects[i]->num_triangles)
			scene->triangle_ref[num_triangles] = &scene->objects[i]->triangles[j++];
		scene->num_triangles += scene->objects[i]->num_triangles;
	}
	update_scene_triangle_tree(scene);
}

static void		update_triangle_normals(t_triangle *triangle)
{
	t_vec3	ab;
	t_vec3	ac;

	ml_vector3_sub(triangle->vtc[1]->pos, triangle->vtc[0]->pos, ab);
	ml_vector3_sub(triangle->vtc[2]->pos, triangle->vtc[0]->pos, ac);
	ml_vector3_cross(ab, ac, triangle->normal);
}

void			transform_3d_object(t_3d_object *obj, t_mat4 transform)
{
	int		i;
	int		j;

	i = -1;
	while (++i < obj->num_vertices)
		ml_matrix4_mul_vec3(transform,
			obj->vertices[i]->pos, obj->vertices[i]->pos);
	j = -1;
	while (++j < obj->num_triangles)
		update_triangle_normals(&obj->triangles[j]);
}

void			destroy_object(t_3d_object *object)
{
	int		i;

	free(object->triangles);
	i = -1;
	while (++i < object->num_vertices)
		free(object->vertices[i]);
	free(object->vertices);
	free(object);
	object = NULL;
}
