/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kd_tree_ray.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/23 18:10:29 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/23 21:31:11 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "kd_tree.h"

static float	min(float n1, float n2)
{
	return (ft_min_double((double[2]){n1, 2}, 2));
}

static float	max(float n1, float n2)
{
	return (ft_max_double((double[2]){n1, 2}, 2));
}

t_bool			kd_tree_bounding_box_hit(t_box3d *box, t_ray *ray,
				t_vec3 out_intersection)
{
	t_vec3		dir_frac;
	float		t_min;
	float		t_max;
	float		t_options[6];
	t_vec3		dir_dist;

	dir_frac[0] = 1.0f / (ray->dir[0] + 0.000001);
	dir_frac[1] = 1.0f / (ray->dir[1] + 0.000001);
	dir_frac[2] = 1.0f / (ray->dir[2] + 0.000001);
	t_options[0] = (box->xyz_min[0] - ray->origin[0]) * dir_frac[0];
	t_options[1] = (box->xyz_max[0] - ray->origin[0]) * dir_frac[0];
	t_options[2] = (box->xyz_min[1] - ray->origin[1]) * dir_frac[1];
	t_options[3] = (box->xyz_max[1] - ray->origin[1]) * dir_frac[1];
	t_options[4] = (box->xyz_min[2] - ray->origin[2]) * dir_frac[2];
	t_options[5] = (box->xyz_max[2] - ray->origin[2]) * dir_frac[2];
	t_min = max(max(min(t_options[0], t_options[1]), min(t_options[2], t_options[3])),
		min(t_options[4], t_options[5]));
	t_max = min(min(max(t_options[0], t_options[1]), max(t_options[2], t_options[3])),
		max(t_options[4], t_options[5]));
	if (t_max < 0)
		return (false);
	if (t_min > t_max)
		return (false);
	ml_vector3_mul(ray->dir, t_min, dir_dist);
	ml_vector3_add(ray->origin, dir_dist, out_intersection);
	return (true);
}

t_bool			kd_tree_triangle_hit(t_triangle *triangle, t_ray *ray,
				t_vec3 out_intersection)
{

}

t_bool			kd_tree_ray_hits(t_kd_node *node, t_ray *ray, float max_dist,
				t_vec3 hit_point)
{
	t_bool	hit_triangle;
	t_bool	hit_right;
	t_bool	hit_left;
	int		i;

	hit_triangle = false;
	hit_right = false;
	hit_left = false;
	if (kd_tree_bounding_box_hit(&node->bounding_box, ray, hit_point))
	{
		if (node->left)
			hit_left = kd_tree_ray_hits(node->left, ray, max_dist);
		if (node->right)
			hit_right = kd_tree_ray_hits(node->right, ray, max_dist);
			return (hit_left || hit_right);
	}
	else
	{
		i = -1;
		while (++i < node->triangles->size)
		{
			if (kd_tree_triangle_hit(node->triangles->triangles[i], ray,
				hit_point))
				hit_triangle = true;
		}
		if (hit_triangle)
			return (true);
	}
	return (false);
}
