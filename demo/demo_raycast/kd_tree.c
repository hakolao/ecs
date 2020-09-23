/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kd_tree.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 21:54:05 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/23 14:21:00 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "obj3d.h"
#include "ft_printf.h"

static void			get_bounding_box(t_tri_vec *triangles, t_box3d res)
{
	int		i;
	int		j;
	float	min;
	float	max;

	if (triangles->size == 0)
	{
		ft_memset(res.xyz_min, 0, sizeof(t_vec3));
		ft_memset(res.xyz_max, 0, sizeof(t_vec3));
	}
	i = -1;
	while (++i < (int)triangles->size)
	{
		j = -1;
		while (++j < 3)
		{
			min = ft_min_double((double[3]){
				triangles->triangles[i]->vtc[0]->pos[j],
				triangles->triangles[i]->vtc[1]->pos[j],
				triangles->triangles[i]->vtc[2]->pos[j]}, 3);
			res.xyz_min[j] = res.xyz_min[j] < min ? res.xyz_min[j] : min;
			max = ft_max_double((double[3]){
				triangles->triangles[i]->vtc[0]->pos[j],
				triangles->triangles[i]->vtc[1]->pos[j],
				triangles->triangles[i]->vtc[2]->pos[j]}, 3);
			res.xyz_max[j] = res.xyz_max[j] > max ? res.xyz_max[j] : max;
		}
	}
	ml_vector3_copy((t_vec3){res.xyz_max[0] - res.xyz_min[0],
		res.xyz_max[1] - res.xyz_min[1], res.xyz_max[2] - res.xyz_min[2],
	}, res.size);
	ml_vector3_copy((t_vec3){res.xyz_min[0] + res.size[0] / 2,
		res.xyz_min[1] + res.size[1] / 2, res.xyz_min[2] + res.size[2] / 2,
	}, res.center);
}

int					get_longest_axis(t_box3d bounding_box)
{
	float	longest;
	int		i;

	longest = ft_max_double((double*)bounding_box.size, 3);
	i = -1;
	while (++i < 3)
		if (bounding_box.size[i] == longest)
			break ;
	return (i);
}

static void			get_mid_point(t_tri_vec *triangles, t_vec3 res)
{
	int		i;

	ft_memset(res, 0, sizeof(t_vec3));
	i = -1;
	while (++i < (int)triangles->size)
	{
		res[0] += triangles->triangles[i]->center[0] * 1.0 / triangles->size;
		res[1] += triangles->triangles[i]->center[1] * 1.0 / triangles->size;
		res[2] += triangles->triangles[i]->center[2] * 1.0 / triangles->size;
	}
}

static t_kd_node	*tree_node_create(t_tri_vec *triangles)
{
	t_kd_node	*node;

	if (!(node = malloc(sizeof(t_kd_node))))
		return (NULL);
	node->bounding_box = (t_box3d){.xyz_max = {FLT_MIN, FLT_MIN, FLT_MIN},
		.xyz_min = {FLT_MAX, FLT_MAX, FLT_MAX}, .center = {0, 0, 0},
		.size = {0, 0, 0}};
	get_bounding_box(triangles, node->bounding_box);
	node->triangles = triangles;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

t_kd_node			*tree_create_recursive(t_tri_vec *triangles, uint32_t depth)
{
	t_vec3		mid_point;
	int32_t		axis;
	t_kd_node	*node;

	node = tree_node_create(triangles);
	if (triangles->size == 0)
		return (node);
	if (triangles->size == 1)
	{
		node->left = tree_node_create(triangle_vec_empty());
		node->right = tree_node_create(triangle_vec_empty());
		return (node);
	}
	get_mid_point(triangles, mid_point);
	axis = get_longest_axis(node->bounding_box);
}

t_kd_tree			*kd_tree_create(t_triangle **triangles,
					uint32_t num_triangles)
{
	t_kd_tree		*tree;
	t_tri_vec		*triangle_vector;

	triangle_vector = triangle_vec(triangles, num_triangles);
	if (!(tree = malloc(sizeof(t_kd_tree))))
		return (NULL);
	tree->num_nodes = 0;
	tree->root = tree_create_recursive(triangle_vector, 0);
	tree->num_nodes++;
	return (tree);
}
