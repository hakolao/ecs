/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   l3d_bounding_box.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 15:03:24 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/29 16:11:23 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib3d.h"

static void			bounding_box_null(t_box3d *res)
{
	ft_memset(res->xyz_min, 0, sizeof(t_vec3));
	ft_memset(res->xyz_max, 0, sizeof(t_vec3));
	ft_memset(res->center, 0, sizeof(t_vec3));
	ft_memset(res->size, 0, sizeof(t_vec3));
}

static void			bounding_box_size_and_center_set(t_box3d *res)
{
	ml_vector3_copy((t_vec3){res->xyz_max[0] - res->xyz_min[0],
		res->xyz_max[1] - res->xyz_min[1], res->xyz_max[2] - res->xyz_min[2],
	}, res->size);
	ml_vector3_copy((t_vec3){res->xyz_min[0] + res->size[0] / 2.,
		res->xyz_min[1] + res->size[1] / 2., res->xyz_min[2] + res->size[2] / 2.,
	}, res->center);
}

void				l3d_bounding_box_set(t_tri_vec *triangles, t_box3d *res)
{
	int		i;
	int		j;
	float	curr_min;
	float	curr_max;

	ml_vector3_copy((t_vec3){INT32_MIN, INT32_MIN, INT32_MIN}, res->xyz_max);
	ml_vector3_copy((t_vec3){INT32_MAX, INT32_MAX, INT32_MAX}, res->xyz_min);
	if (triangles->size == 0)
		bounding_box_null(res);
	i = -1;
	while (++i < (int)triangles->size)
	{
		j = -1;
		while (++j < 3)
		{
			curr_max = l3d_fmin(triangles->triangles[i]->vtc[0]->pos[j], l3d_fmin(
				triangles->triangles[i]->vtc[1]->pos[j],
				triangles->triangles[i]->vtc[2]->pos[j]));
			res->xyz_min[j] = res->xyz_min[j] < curr_min ?
				res->xyz_min[j] : curr_min;
			curr_max = l3d_fmax(triangles->triangles[i]->vtc[0]->pos[j], l3d_fmax(
				triangles->triangles[i]->vtc[1]->pos[j],
				triangles->triangles[i]->vtc[2]->pos[j]));
			res->xyz_max[j] = res->xyz_max[j] > curr_max ?
				res->xyz_max[j] : curr_max;
		}
	}
	bounding_box_size_and_center_set(res);
}

t_axis				l3d_bounding_box_longest_axis(t_box3d bounding_box)
{
	float	longest;
	int		i;

	longest = (float)ft_max_double((double[3]){
		bounding_box.size[0], bounding_box.size[1], bounding_box.size[2]}, 3);
	i = -1;
	while (++i < 3)
		if (bounding_box.size[i] == longest)
			break ;
	return (i);
}
