/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_map_delete.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/12 16:47:53 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/15 19:55:09 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hash_map.h"
#include "hash_map_utils.h"

/*
** Deletes an element in a hash table, freeing its memory.
** Loop through nodes of a hash map table bucket starting at t_hash_node *curr.
** Delete found node and place the next of a previous node to be the following
** node of curr.
*/

t_hash_node			*hash_map_find_and_null_node(int key,
					t_hash_node *prev, t_hash_node *curr,
					t_hash_node *next)
{
	while (curr)
	{
		if (curr->key == key)
		{
			if (next == NULL)
			{
				if (prev != NULL)
					prev->next = NULL;
				return (curr);
			}
			prev->next = next;
			return (curr);
		}
		prev = curr;
		curr = curr->next;
		if (curr)
			next = curr->next;
	}
	return (NULL);
}

/*
** Delete a t_hash_node in a hash map if found.
*/

void				hash_map_delete(t_hash_table *table, int key)
{
	int			pos;
	t_hash_node	*curr;
	t_hash_node	*next;
	t_hash_node	*prev;

	pos = hash_map_hash(table, key);
	curr = table->list[pos];
	prev = NULL;
	next = NULL;
	curr = hash_map_find_and_null_node(key, prev, curr, next);
	if (curr != NULL)
	{
		free(curr);
		curr = NULL;
	}
}

/*
** Clear elements inside hashmap, without freeing table memory.
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
