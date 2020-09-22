/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kd_tree.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 21:54:05 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/23 00:32:02 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "kd_tree.h"
#include "ft_printf.h"

static t_kd_node	*tree_node_create(t_triangle **triangles,
					uint32_t num_triangles, uint32_t uuid)
{
	t_kd_node	*node;
	int32_t		i;

	if (!(node = malloc(sizeof(t_kd_node))))
		return (NULL);
	i = -1;
	//ToDo
	(void)triangles;
	(void)num_triangles;
	node->uuid = uuid;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

t_kd_tree			*kd_tree_create(t_triangle **triangles,
					uint32_t num_triangles)
{
	t_kd_tree		*tree;

	if (!(tree = malloc(sizeof(t_kd_tree))))
		return (NULL);
	tree->root = tree_node_create(triangles, num_triangles, 0);
	tree->num_nodes++;
	tree->next_uuid++;
	return (tree);
}
