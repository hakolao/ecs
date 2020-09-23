/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kd_tree_ray.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/23 18:10:29 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/23 21:52:12 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "kd_tree.h"

static float	min(float n1, float n2)
{
	return (ft_min_double((double[2]){n1, n2}, 2));
}

static float	max(float n1, float n2)
{
	return (ft_max_double((double[2]){n1, n2}, 2));
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
	t_vec3	edge1;
	t_vec3	edge2;
	t_vec3	dir_dist;
	t_vec3	hsq[3];
	float	afuvt[5];

	ml_vector3_sub(triangle->vtc[1]->pos, triangle->vtc[0]->pos, edge1);
	ml_vector3_sub(triangle->vtc[2]->pos, triangle->vtc[0]->pos, edge2);
	ml_vector3_cross(ray->dir, edge2, hsq[0]);
	afuvt[0] = ml_vector3_dot(edge1, hsq[0]);
	if (afuvt[0] > -EPSILON && afuvt[0] < EPSILON)
		return (false);
	afuvt[1] = 1.0 / afuvt[0];
	ml_vector3_sub(ray->origin, triangle->vtc[0]->pos, hsq[1]);
	afuvt[2] = afuvt[1] * ml_vector3_dot(hsq[1], hsq[0]);
	if (afuvt[2] < 0.0 || afuvt[2] > 1.0)
		return (false);
	ml_vector3_cross(hsq[1], edge1, hsq[2]);
	afuvt[3] = afuvt[1] *  ml_vector3_dot(ray->dir, hsq[2]);
	if (afuvt[3] < 0.0 || afuvt[2] + afuvt[3] > 1.0)
		return (false);
	afuvt[4] = afuvt[1] * ml_vector3_dot(edge2, hsq[2]);
	if (afuvt[4] > EPSILON)
	{
		ml_vector3_mul(ray->dir, afuvt[4], dir_dist);
		ml_vector3_add(ray->origin, dir_dist, out_intersection);
		return (true);
	}
	return (false);
}

t_bool			kd_tree_ray_hit(t_kd_node *node, t_ray *ray, float max_dist,
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
			hit_left = kd_tree_ray_hit(node->left, ray, max_dist, hit_point);
		if (node->right)
			hit_right = kd_tree_ray_hit(node->right, ray, max_dist, hit_point);
			return (hit_left || hit_right);
	}
	else
	{
		i = -1;
		while (++i < (int)node->triangles->size)
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
