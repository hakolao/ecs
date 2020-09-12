/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_map_delete_free.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/12 17:25:46 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/12 17:35:08 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hash_map.h"
#include "hash_map_utils.h"

/*
** Delete a t_hash_node in a hash map if found.
** Also frees value. If value has not been malloced, a crash may occur.
*/

void				hash_map_delete_free(t_hash_table *table, int key)
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
	free(curr->val);
	free(curr);
	curr = NULL;
}

/*
** Clear elements inside hashmap, without freeing table memory.
** Also frees values. If value(s) has not been malloced, a crash may occur.
*/

void				hash_map_clear_free(t_hash_table *table)
{
	size_t		i;
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
			free(temp->val);
			free(temp);
			temp = NULL;
		}
		table->list[i] = NULL;
	}
}

/*
** Destroy entire hash table freeing its memory.
** Also frees values. If value(s) has not been malloced, a crash may occur.
*/

void				hash_map_destroy_free(t_hash_table *table)
{
	hash_map_clear_free(table);
	free(table->list);
	free(table);
}
