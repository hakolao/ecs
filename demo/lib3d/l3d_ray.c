/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   l3d_ray.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/23 18:10:29 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/29 15:33:27 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib3d.h"

t_bool			l3d_bounding_box_ray_hit(t_box3d *box, t_ray *ray, t_hit *hit)
{
	float 	t[9];

	t[1] = (box->xyz_min[0] - ray->origin[0]) * ray->dir_inv[0];
	t[2] = (box->xyz_max[0] - ray->origin[0]) * ray->dir_inv[0];
	t[3] = (box->xyz_min[1] - ray->origin[1]) * ray->dir_inv[1];
	t[4] = (box->xyz_max[1] - ray->origin[1]) * ray->dir_inv[1];
	t[5] = (box->xyz_min[2] - ray->origin[2]) * ray->dir_inv[2];
	t[6] = (box->xyz_max[2] - ray->origin[2]) * ray->dir_inv[2];
	t[7] = l3d_fmax(l3d_fmax(l3d_fmin(t[1], t[2]), l3d_fmin(t[3], t[4])), l3d_fmin(t[5], t[6]));
	t[8] = l3d_fmin(l3d_fmin(l3d_fmax(t[1], t[2]), l3d_fmax(t[3], t[4])), l3d_fmax(t[5], t[6]));
	if (t[8] < 0 || t[7] > t[8])
		return (false);
	hit->t = t[7];
	return (true);
}

t_bool			l3d_triangle_ray_hit(t_triangle *triangle, t_ray *ray, t_hit *hit)
{
	t_vec3	edge1;
	t_vec3	edge2;
	t_vec3	n;
	t_vec3	hsq[3];
	float	afuvt[5];

	ml_vector3_sub(triangle->vtc[1]->pos, triangle->vtc[0]->pos, edge1);
	ml_vector3_sub(triangle->vtc[2]->pos, triangle->vtc[0]->pos, edge2);
	ml_vector3_cross(ray->dir, edge2, hsq[0]);
	ml_vector3_cross(edge1, edge2, n);
	ml_vector3_normalize(n, n);
	if (ml_vector3_dot(ray->dir, n) > 0 && triangle->is_single_sided)
		return (false);
	afuvt[0] = ml_vector3_dot(edge1, hsq[0]);
	if (afuvt[0] > -L3D_EPSILON && afuvt[0] < L3D_EPSILON)
		return (false);
	afuvt[1] = 1.0 / afuvt[0];
	ml_vector3_sub(ray->origin, triangle->vtc[0]->pos, hsq[1]);
	afuvt[2] = afuvt[1] * ml_vector3_dot(hsq[1], hsq[0]);
	if (afuvt[2] < 0.0 || afuvt[2] > 1.0)
		return (false);
	ml_vector3_cross(hsq[1], edge1, hsq[2]);
	afuvt[3] = afuvt[1] * ml_vector3_dot(ray->dir, hsq[2]);
	if (afuvt[3] < 0.0 || afuvt[2] + afuvt[3] > 1.0)
		return (false);
	afuvt[4] = afuvt[1] * ml_vector3_dot(edge2, hsq[2]);
	if (afuvt[4] > L3D_EPSILON)
	{
		hit->t = afuvt[4];
		return (true);
	}
	return (false);
}

t_bool			l3d_kd_tree_ray_hit(t_kd_node *node, t_ray *ray, float t_max, t_hit *hit)
{
	t_bool	hit_triangle;
	t_vec3	dir_add;
	t_bool	hits_right;
	t_bool	hits_left;
	int		i;

	hit_triangle = false;
	if (l3d_bounding_box_ray_hit(&node->bounding_box, ray, hit))
	{
		if (node->left || node->right)
		{
			hits_left = l3d_kd_tree_ray_hit(node->left, ray, t_max, hit);
			hits_right = l3d_kd_tree_ray_hit(node->right, ray, t_max, hit);
			return (hits_left || hits_right);
		}
		i = -1;
		while (++i < (int)node->triangles->size)
		{
			if (l3d_triangle_ray_hit(node->triangles->triangles[i], ray, hit))
			{
				hit_triangle = true;
				ml_vector3_mul(ray->dir, hit->t, dir_add);
				ml_vector3_add(ray->origin, dir_add, hit->hit_point);
				ml_vector3_copy(node->triangles->triangles[i]->normal,
					hit->normal);
				t_max = hit->t;
			}
		}
		return (hit_triangle);
	}
	return (false);
}

void						l3d_ray_set(t_vec3 dir, t_vec3 origin, t_ray *ray)
{
	ml_vector3_set(ray->dir, dir[0], dir[1], dir[2]);
	ml_vector3_set(ray->origin, origin[0], origin[1], origin[2]);
	ml_vector3_normalize(ray->dir, ray->dir);
	ml_vector3_set(ray->dir_inv, 1.0 / dir[0], 1.0 / dir[1], 1.0 / dir[2]);
}
