/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_map_delete.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/12 16:47:53 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/15 22:50:40 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hash_map.h"
#include "ft_printf.h"

/*
** Delete a t_hash_node in a hash map if found.
*/

void				hash_map_delete(t_hash_table *table, int key)
{
	int			pos;
	t_hash_node	**curr;
	t_hash_node	*temp;

	pos = hash_map_hash(table, key);
	curr = &table->list[pos];
	while (*curr)
	{
		temp = *curr;
		if (temp->key == key)
		{
			*curr = temp->next;
			free(temp);
			break ;
		}
		else
			curr = &(*curr)->next;
	}
}

/*
** Clear elements inside hashmap, without freeing table values' memory.
** Use when elements were not malloced.
*/

void				hash_map_clear(t_hash_table *table)
{
	int			i;
	t_hash_node	*temp;
	t_hash_node	*curr;

	i = -1;
	while (++i < table->size)
	{
		curr = table->list[i];
		while (curr)
		{
			temp = curr;
			curr = curr->next;
			free(temp);
			temp = NULL;
		}
		table->list[i] = NULL;
	}
}

/*
** Destroy entire hash table freeing its memory.
*/

void				hash_map_destroy(t_hash_table *table)
{
	hash_map_clear(table);
	free(table->list);
	free(table);
}
