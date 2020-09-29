/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   l3d_triangle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 14:55:40 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/29 15:44:43 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib3d.h"

void			l3d_triangle_centroid(t_triangle *triangle)
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

void				l3d_triangles_midpoint(t_tri_vec *triangles, t_vec3 res)
{
	int		i;

	ft_memset(res, 0, sizeof(t_vec3));
	i = -1;
	while (++i < (int)triangles->size)
	{
		res[0] += triangles->triangles[i]->center[0] / triangles->size;
		res[1] += triangles->triangles[i]->center[1] / triangles->size;
		res[2] += triangles->triangles[i]->center[2] / triangles->size;
	}
}

void				l3d_triangle_normal_set(t_triangle *triangle)
{
	t_vec3	ab;
	t_vec3	ac;

	ml_vector3_sub(triangle->vtc[1]->pos, triangle->vtc[0]->pos, ab);
	ml_vector3_sub(triangle->vtc[2]->pos, triangle->vtc[0]->pos, ac);
	ml_vector3_cross(ab, ac, triangle->normal);
}
