/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_map_utils.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/12 17:31:57 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/12 17:33:23 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HASH_MAP_UTILS_H
# define HASH_MAP_UTILS_H

#include "hash_map.h"

t_hash_node			*hash_map_find_and_null_node(int key,
					t_hash_node *prev, t_hash_node *curr,
					t_hash_node *next);

#endif
