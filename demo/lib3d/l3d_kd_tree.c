/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   l3d_kd_tree.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 21:54:05 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/29 21:38:16 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib3d_internals.h"

static void			kd_node_destroy(t_kd_node *root)
{
	t_kd_node	*left;
	t_kd_node	*right;
	if (root)
	{
		l3d_triangle_vec_delete(root->triangles);
		left = root->left;
		right = root->right;
		free(root);
		root = NULL;
		kd_node_destroy(left);
		kd_node_destroy(right);
	}
}

static t_kd_node	*tree_node_create(t_tri_vec *triangles)
{
	t_kd_node	*node;

	if (!(node = malloc(sizeof(t_kd_node))))
		return (NULL);
	l3d_bounding_box_set(triangles, &node->bounding_box);
	node->triangles = triangles;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

static t_kd_node	*tree_create_recursive(t_tri_vec *triangles, uint32_t depth,
					uint32_t *num_nodes)
{

	t_kd_node	*node;
	t_tri_vec	*left_tris;
	t_tri_vec	*right_tris;

	node = tree_node_create(triangles);
	node->uuid = (*num_nodes)++;
	if ((triangles->size == 0 || triangles->size == 1) &&
		(node->axis = l3d_axis_none))
		return (node);
	node->axis = l3d_bounding_box_longest_axis(node->bounding_box);
	left_tris = l3d_triangle_vec_empty();
	right_tris = l3d_triangle_vec_empty();
	l3d_kd_tree_split_triangles(triangles, node->axis, left_tris, right_tris);
	if ((left_tris->size < L3D_MIN_KD_NODE_NUM_TRIANGLES ||
		right_tris->size < L3D_MIN_KD_NODE_NUM_TRIANGLES))
	{
		l3d_triangle_vec_delete(left_tris);
		l3d_triangle_vec_delete(right_tris);
		return (node);
	}
	if (depth < L3D_MAX_KD_TREE_DEPTH)
	{
		node->left = tree_create_recursive(left_tris, depth + 1, num_nodes);
		node->right = tree_create_recursive(right_tris, depth + 1, num_nodes);
	}
	return (node);
}

t_kd_tree			*l3d_kd_tree_create(t_triangle **triangles,
					uint32_t num_triangles)
{
	t_kd_tree		*tree;
	t_tri_vec		*triangle_vector;

	triangle_vector = l3d_triangle_vec(triangles, num_triangles);
	if (!(tree = malloc(sizeof(t_kd_tree))))
		return (NULL);
	tree->num_nodes = 0;
	tree->root = tree_create_recursive(triangle_vector, 0, &tree->num_nodes);
	return (tree);
}

void				l3d_kd_tree_destroy(t_kd_tree *tree)
{
	kd_node_destroy(tree->root);
	free(tree);
	tree = NULL;
}

void				l3d_kd_tree_create_or_update(t_kd_tree **tree,
					t_triangle **triangles, uint32_t num_triangles)
{
	if (*tree)
		l3d_kd_tree_destroy(*tree);
	*tree = l3d_kd_tree_create(triangles, num_triangles);
}
