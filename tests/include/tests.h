/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/16 19:23:54 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/30 01:13:43 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TESTS_H
# define TESTS_H

const char		*test_hash_map_create(void);
const char		*test_hash_map_add(void);
const char		*test_hash_map_get(void);
const char		*test_hash_map_delete(void);
const char		*test_hash_map_has_key(void);
const char		*test_hash_map_foreach(void);
const char		*test_world_create(void);
const char		*test_world_system_add(void);
const char		*test_world_system_remove(void);
const char		*test_world_component_add(void);
const char		*test_world_component_remove(void);
const char		*test_world_entity_add(void);
const char		*test_world_entity_remove(void);
const char		*test_world_entity_component_add(void);
const char		*test_world_entity_utils(void);

#endif
