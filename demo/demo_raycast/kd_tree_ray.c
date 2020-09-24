/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kd_tree_ray.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/23 18:10:29 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/24 13:44:39 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "kd_tree.h"

static float	min(float n1, float n2)
{
	return (n1 < n2 ? n1 : n2);
}

static float	max(float n1, float n2)
{
	return (n1 > n2 ? n1 : n2);
}

t_bool			kd_tree_bounding_box_hit(t_box3d *box, t_ray *ray,
				t_vec3 out_intersection)
{
	float 	t[9];
	t_vec3	dir_dist;

	t[1] = (box->xyz_min[0] - ray->origin[0]) / ray->dir[0];
	t[2] = (box->xyz_max[0] - ray->origin[0]) / ray->dir[0];
	t[3] = (box->xyz_min[1] - ray->origin[1]) / ray->dir[1];
	t[4] = (box->xyz_max[2] - ray->origin[1]) / ray->dir[1];
	t[5] = (box->xyz_min[2] - ray->origin[2]) / ray->dir[2];
	t[6] = (box->xyz_max[2] - ray->origin[2]) / ray->dir[2];
	t[7] = max(max(min(t[1], t[2]), min(t[3], t[4])), min(t[5], t[6]));
	t[8] = min(min(max(t[1], t[2]), max(t[3], t[4])), max(t[5], t[6]));
	if ((t[8] < 0 || t[7] > t[8]))
		return (false);
	ml_vector3_mul(ray->dir, t[7], dir_dist);
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
				t_vec3 hit_p)
{
	t_bool	hit_triangle;
	int		i;

	hit_triangle = false;
	if (kd_tree_bounding_box_hit(&node->bounding_box, ray, hit_p))
	{
		if (node->left || node->right)
			return (kd_tree_ray_hit(node->left, ray, max_dist, hit_p) ||
				kd_tree_ray_hit(node->right, ray, max_dist, hit_p));
		i = -1;
		while (++i < (int)node->triangles->size)
			if (kd_tree_triangle_hit(node->triangles->triangles[i], ray, hit_p))
				hit_triangle = true;
		return (hit_triangle);
	}
	return (false);
}
