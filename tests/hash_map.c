/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/15 19:39:09 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/15 21:16:09 by ohakola          ###   ########.fr       */
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
	hash_map_destroy_free(hash_map);
	return (0);
}

const char		*test_hash_map_add(void)
{
	t_hash_table	*hash_map;
	int				size;
	int				key;
	int				hashed_key;
	const char		*result_str;

	size = 128;
	hash_map = hash_map_create(size);
	key = (int)"okko";
	hashed_key = hash_map_hash(hash_map, key);
	hash_map_add(hash_map, key, (void*)"an aristocrat");
	result_str = (char*)hash_map->list[hashed_key]->val;
	oh_assert("Hash map add is not correct",
		ft_strequ(result_str, "an aristocrat"));
	hash_map_destroy(hash_map);
	return (0);
}

const char		*test_hash_map_get(void)
{
	t_hash_table	*hash_map;
	int				size;
	int				key;

	size = 128;
	hash_map = hash_map_create(size);
	key = (int)"okko";
	hash_map_add(hash_map, key, (void*)"an aristocrat");
	oh_assert("Hash map get is not correct",
		ft_strequ((char*)hash_map_get(hash_map, key), "an aristocrat"));
	key = (int)"marsupilami";
	hash_map_add(hash_map, key, (void*)"an automat");
	oh_assert("Hash map get is not correct",
		ft_strequ((char*)hash_map_get(hash_map, key), "an automat"));
	hash_map_destroy(hash_map);
	return (0);
}
