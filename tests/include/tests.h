/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/16 19:23:54 by ohakola           #+#    #+#             */
/*   Updated: 2020/10/13 12:57:52 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TESTS_H
# define TESTS_H

const char		*test_world_create(void);
const char		*test_world_system_add(void);
const char		*test_world_system_remove(void);
const char		*test_world_system_run(void);
const char		*test_world_system_run_many(void);
const char		*test_world_system_run_many_entities(void);
const char		*test_world_system_run_many_per_thread(void);
const char		*test_world_component_add(void);
const char		*test_world_component_remove(void);
const char		*test_world_entity_add(void);
const char		*test_world_entity_remove(void);
const char		*test_world_entity_component_add(void);
const char		*test_world_entity_utils(void);

#endif
