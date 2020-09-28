/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle_vector.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/23 13:53:28 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/28 14:52:34 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "kd_tree.h"

t_tri_vec		*triangle_vec_empty(void)
{
	t_tri_vec	*vector;

	if ((!(vector = malloc(sizeof(t_tri_vec))) ||
		!(vector->triangles = malloc(sizeof(t_triangle*) *
			TRI_VEC_INITIAL_CAPACITY))) &&
		ft_dprintf(2, "Failed to malloc triangle vector\n"))
		exit(1);
	vector->size = 0;
	vector->capacity = TRI_VEC_INITIAL_CAPACITY;
	return (vector);
}

t_tri_vec		*triangle_vec(t_triangle **triangles, uint32_t num_triangles)
{
	t_tri_vec	*vector;
	uint32_t	capacity;
	int			i;

	capacity = num_triangles > TRI_VEC_INITIAL_CAPACITY ?
		num_triangles + TRI_VEC_INITIAL_CAPACITY : TRI_VEC_INITIAL_CAPACITY;
	if ((!(vector = malloc(sizeof(t_tri_vec))) ||
		!(vector->triangles = malloc(sizeof(t_triangle*) * capacity))) &&
		ft_dprintf(2, "Failed to malloc triangle vector\n"))
		exit(1);
	i = -1;
	while (++i < (int)num_triangles)
		vector->triangles[i] = triangles[i];
	vector->size = num_triangles;
	vector->capacity = capacity;
	return (vector);
}

void			triangle_vec_push(t_tri_vec *vector, t_triangle *triangle)
{
	t_triangle	*temp[vector->size];
	uint32_t	new_capacity;
	int			i;

	if (vector->size < vector->capacity)
		vector->triangles[vector->size++] = triangle;
	else
	{
		new_capacity = vector->capacity * 2;
		i = -1;
		while (++i < (int)vector->size)
			temp[i] = vector->triangles[i];
		free(vector->triangles);
		if (!(vector->triangles = malloc(sizeof(t_triangle*) * new_capacity)) &&
			ft_dprintf(2, "Failed to malloc triangle vector new size\n"))
			exit(1);
		i = -1;
		while (++i < (int)vector->size)
			vector->triangles[i] = temp[i];
		vector->triangles[vector->size++] = triangle;
		vector->capacity = new_capacity;
	}
}

void			tri_vec_delete(t_tri_vec *vector)
{
	free(vector->triangles);
	free(vector);
}
