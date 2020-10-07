/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   l3d_obj_read_utils2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 22:43:43 by ohakola           #+#    #+#             */
/*   Updated: 2020/10/07 15:28:56 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib3d_internals.h"

/*
** Our obj file reader expects triangulated data, thus this validator tests
** number of vertices, textures and normals equals max incides read from obj
** data.
*/

static t_bool			l3d_is_valid_obj(t_obj *obj, uint32_t prev[3])
{
	int				i;
	int				j;
	uint32_t		curr[3];

	i = -1;
	curr[0] = 0;
	curr[1] = 0;
	curr[2] = 0;
	while (++i < (int)obj->num_triangles)
	{
		j = -1;
		while (++j < 3)
		{
			curr[0] = curr[0] > obj->triangles[i * 9 + j * 3 + 0] ?
				curr[0] : obj->triangles[i * 9 + j * 3 + 0];
			curr[1] = curr[1] > obj->triangles[i * 9 + j * 3 + 1] ?
				curr[1] : obj->triangles[i * 9 + j * 3 + 1];
			curr[2] = curr[2] > obj->triangles[i * 9 + j * 3 + 2] ?
				curr[2] : obj->triangles[i * 9 + j * 3 + 2];
		}
	}
	return (curr[0] - prev[0] == obj->num_vertices &&
		curr[1] - prev[1] == obj->num_v_text_coords &&
		curr[2] - prev[2] == obj->num_v_normals);
}

/*
** Validates a list of t_objs. The vertex indices on obj files don't zero
** after objects thus the prev & current checks above and below.
** There's a chanse this is wrong if objects can share vertices in the obj file.
*/

t_bool					l3d_is_valid_obj_content(t_obj_content *obj_content)
{
	int				i;
	uint32_t		vtn_indices[3];

	i = -1;
	vtn_indices[0] = 0;
	vtn_indices[1] = 0;
	vtn_indices[2] = 0;
	while (++i < (int)obj_content->num_objects)
	{
		if (i > 0)
		{
			vtn_indices[0] += obj_content->objects[i - 1].num_vertices;
			vtn_indices[1] += obj_content->objects[i - 1].num_v_text_coords;
			vtn_indices[2] += obj_content->objects[i - 1].num_v_normals;
		}
		error_check(!l3d_is_valid_obj(&obj_content->objects[i], vtn_indices),
			"Invalid number of v, vt, vn or triangles");
	}
	return (true);
}
