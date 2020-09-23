/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kd_tree.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 21:54:05 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/23 17:54:25 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "obj3d.h"
#include "ft_printf.h"

static void			get_bounding_box(t_tri_vec *triangles, t_box3d *res)
{
	int		i;
	int		j;
	float	min;
	float	max;

	ml_vector3_copy((t_vec3){FLT_MIN, FLT_MIN, FLT_MIN}, res->xyz_max);
	ml_vector3_copy((t_vec3){FLT_MAX, FLT_MAX, FLT_MAX}, res->xyz_min);
	if (triangles->size == 0)
	{
		ft_memset(res->xyz_min, 0, sizeof(t_vec3));
		ft_memset(res->xyz_max, 0, sizeof(t_vec3));
		ft_memset(res->center, 0, sizeof(t_vec3));
		ft_memset(res->size, 0, sizeof(t_vec3));
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
			res->xyz_min[j] = res->xyz_min[j] < min ? res->xyz_min[j] : min;
			max = ft_max_double((double[3]){
				triangles->triangles[i]->vtc[0]->pos[j],
				triangles->triangles[i]->vtc[1]->pos[j],
				triangles->triangles[i]->vtc[2]->pos[j]}, 3);
			res->xyz_max[j] = res->xyz_max[j] > max ? res->xyz_max[j] : max;
		}
	}
	ml_vector3_copy((t_vec3){res->xyz_max[0] - res->xyz_min[0],
		res->xyz_max[1] - res->xyz_min[1], res->xyz_max[2] - res->xyz_min[2],
	}, res->size);
	ml_vector3_copy((t_vec3){res->xyz_min[0] + res->size[0] / 2.,
		res->xyz_min[1] + res->size[1] / 2., res->xyz_min[2] + res->size[2] / 2.,
	}, res->center);
}

int					get_longest_axis(t_box3d bounding_box)
{
	float	longest;
	int		i;

	longest = ft_max_double((double[3]){
		bounding_box.size[0], bounding_box.size[1], bounding_box.size[2]
	}, 3);
	i = -1;
	while (++i < 3)
		if (bounding_box.size[i] - 0.00001 < longest &&
			bounding_box.size[i] + 0.00001 > longest)
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
	get_bounding_box(triangles, &node->bounding_box);
	node->triangles = triangles;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

void				split_triangles(t_tri_vec *triangles, uint32_t axis,
					t_tri_vec *left_tris, t_tri_vec *right_tris)
{
	t_vec3		mid_point;
	int			i;

	get_mid_point(triangles, mid_point);
	i = -1;
	while (++i < (int)triangles->size)
	{
		if (axis == 0)
		{
			if (mid_point[0] >= triangles->triangles[i]->center[0])
				triangle_vec_push(right_tris, triangles->triangles[i]);
			else
				triangle_vec_push(left_tris, triangles->triangles[i]);
		}
		else if (axis == 1)
		{
			if (mid_point[1] >= triangles->triangles[i]->center[1])
				triangle_vec_push(right_tris, triangles->triangles[i]);
			else
				triangle_vec_push(left_tris, triangles->triangles[i]);
		}
		else if (axis == 2)
		{
			if (mid_point[2] >= triangles->triangles[i]->center[2])
				triangle_vec_push(right_tris, triangles->triangles[i]);
			else
				triangle_vec_push(left_tris, triangles->triangles[i]);
		}
	}
}

int					calculate_matches(t_tri_vec *left_tris, t_tri_vec *right_tris)
{
	int		i;
	int		j;
	int		matches;

	matches = 0;
	i = -1;
	while (++i < (int)left_tris->size)
	{
		j = -1;
		while (++j < (int)right_tris->size)
		{
			if (left_tris->triangles[i] == right_tris->triangles[i])
				matches++;
		}
	}
	return (matches);
}

t_kd_node			*tree_create_recursive(t_tri_vec *triangles, uint32_t depth,
					uint32_t *num_nodes)
{

	t_kd_node	*node;
	t_tri_vec	*left_tris;
	t_tri_vec	*right_tris;
	int			matches;

	node = tree_node_create(triangles);
	node->uuid = (*num_nodes)++;
	if (triangles->size == 0 || triangles->size == 1)
		return (node);
	node->axis = get_longest_axis(node->bounding_box);
	left_tris = triangle_vec_empty();
	right_tris = triangle_vec_empty();
	split_triangles(triangles, node->axis, left_tris, right_tris);
	if (left_tris->size == 0 && right_tris->size > 0)
	{
		tri_vec_delete(left_tris);
		left_tris = right_tris;
	}
	if (right_tris->size == 0 && left_tris->size > 0)
	{
		tri_vec_delete(right_tris);
		right_tris = left_tris;
	}
	matches = calculate_matches(left_tris, right_tris);
	if ((float)matches / left_tris->size < 0.5 &&
		(float)matches / right_tris->size < 0.5)
	{
		node->left = tree_create_recursive(left_tris, depth + 1, num_nodes);
		node->right = tree_create_recursive(right_tris, depth + 1, num_nodes);
	}
	return (node);
}

void				kd_tree_print(t_kd_node *root)
{
	if (root != NULL)
	{
		ft_printf("Triangles: %d\n", root->triangles->size);
		ft_printf("uuid: %d\nbbox.center: [%0.2f %0.2f %0.2f]\n"
				"axis: %d\n"
				"bbox.min_xyz: [%0.2f %0.2f %0.2f]\n"
				"bbox.max_xyz: [%0.2f %0.2f %0.2f]\n",
				root->uuid,
				root->bounding_box.center[0],
				root->bounding_box.center[1],
				root->bounding_box.center[2],
				root->axis,
				root->bounding_box.xyz_min[0],
				root->bounding_box.xyz_min[1],
				root->bounding_box.xyz_min[2],
				root->bounding_box.xyz_max[0],
				root->bounding_box.xyz_max[1],
				root->bounding_box.xyz_max[2]);
		kd_tree_print(root->left);
		kd_tree_print(root->right);
	}
}

t_kd_tree			*kd_tree_create(t_triangle *triangles,
					uint32_t num_triangles)
{
	t_kd_tree		*tree;
	t_tri_vec		*triangle_vector;

	triangle_vector = triangle_vec(triangles, num_triangles);
	if (!(tree = malloc(sizeof(t_kd_tree))))
		return (NULL);
	tree->root = tree_create_recursive(triangle_vector, 0, &tree->num_nodes);
	kd_tree_print(tree->root);
	return (tree);
}

static void			node_destroy(t_kd_node *root)
{
	t_kd_node	*left;
	t_kd_node	*right;
	if (root)
	{
		tri_vec_delete(root->triangles);
		left = root->left;
		right = root->right;
		free(root);
		root = NULL;
		node_destroy(left);
		node_destroy(right);
	}
}

void				kd_tree_destroy(t_kd_tree *tree)
{
	node_destroy(tree->root);
	free(tree);
	tree = NULL;
}
