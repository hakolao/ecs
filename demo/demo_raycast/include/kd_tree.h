/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kd_tree.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 21:49:59 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/23 10:57:36 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KD_TREE_H
# define KD_TREE_H

# include <stdlib.h>
# include <float.h>
# include "t_bool.h"
# include "libgmatrix.h"

# define MAX_TRIANGLES 1024

typedef t_vec3			t_triangle[3];

typedef struct	s_kd_node	t_kd_node;

struct			s_kd_node
{
	uint32_t	uuid;
	t_vec3		bounding_box[4];
	t_triangle	*triangles[MAX_TRIANGLES];
	uint32_t	num_triangles;
	t_kd_node	*left;
	t_kd_node	*right;
};

typedef struct				s_kd_tree
{
	uint32_t	num_nodes;
	uint32_t	num_dimensions;
	uint32_t	next_uuid;
	t_kd_node	*root;
}							t_kd_tree;

typedef struct				s_kd_insert
{
	t_kd_node	*root;
	float		*point;
	uint32_t	next_uuid;
	uint32_t	depth;
}							t_kd_insert;

#endif
