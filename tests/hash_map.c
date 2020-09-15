/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/15 19:39:09 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/15 19:58:21 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "oh_test.h"
#include "libft.h"

const char		*test_hash_map_create(void)
{
	t_hash_table	*hash_map;
	int				size;

	size = 128;
	hash_map = NULL;
	hash_map = hash_map_create(size);
	oh_assert("Hash map wasn't created", hash_map != NULL);
	oh_assert("Hash map size is wrong", hash_map->size == size);
	return (0);
}
