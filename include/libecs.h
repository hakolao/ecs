/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libecs.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/17 13:11:01 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/12 17:18:17 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBECS_H
# define LIBECS_H

# include "libft.h"

/*
** ToDo: A library needs to be able give max ents as input, so this won't do
*/

# define MAX_ENTITIES 200

/*
** ToDo: A library needs to be able to add components, so this won't do...
*/

typedef enum		s_component
{
	component_empty = 0,
	component_position = 1,
	component_velocity = (1 << 1),
}					t_component;

typedef struct		s_parts
{
	t_hash_table	*position;
	t_hash_table	*velocity;
	t_hash_table	*something;
}					t_parts;

typedef struct		s_world
{
	uint32_t		entity_masks[MAX_ENTITIES];
	t_parts			parts;
}					t_world;

#endif
