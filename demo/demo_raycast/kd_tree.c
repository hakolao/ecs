/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kd_tree.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 21:54:05 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/28 01:15:20 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "kd_tree.h"
#include "ft_printf.h"

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

static int			calculate_matches(t_tri_vec *left_tris,
					t_tri_vec *right_tris)
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
			if (left_tris->triangles[i] == right_tris->triangles[j])
				matches++;
	}
	return (matches);
}

static t_kd_node	*tree_create_recursive(t_tri_vec *triangles, uint32_t depth,
					uint32_t *num_nodes)
{

	t_kd_node	*node;
	t_tri_vec	*left_tris;
	t_tri_vec	*right_tris;
	int			matches;

	node = tree_node_create(triangles);
	node->uuid = (*num_nodes)++;
	if ((triangles->size == 0 || triangles->size == 1) && (node->axis = 3))
		return (node);
	node->axis = get_bounding_box_longest_axis(node->bounding_box);
	left_tris = triangle_vec_empty();
	right_tris = triangle_vec_empty();
	kd_tree_split_triangles(triangles, node->axis, left_tris, right_tris);
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

t_kd_tree			*kd_tree_create(t_triangle *triangles,
					uint32_t num_triangles)
{
	t_kd_tree		*tree;
	t_tri_vec		*triangle_vector;

	triangle_vector = triangle_vec(triangles, num_triangles);
	if (!(tree = malloc(sizeof(t_kd_tree))))
		return (NULL);
	tree->num_nodes = 0;
	tree->root = tree_create_recursive(triangle_vector, 0, &tree->num_nodes);
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
