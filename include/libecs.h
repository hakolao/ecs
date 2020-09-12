/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libecs.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/17 13:11:01 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/12 19:13:58 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBECS_H
# define LIBECS_H

# include "libft.h"

/*
** 64 (max number of bits in uint64_t (for bitmasks))
*/

# define ECS_MAX_COMPONENTS 64

/*
** Redefine these if you wish more capacity for entity types & systems
*/

# ifndef ECS_MAX_ENTITY_TYPES
#  define ECS_MAX_ENTITY_TYPES 1024
# endif
# ifndef ECS_MAX_SYSTEMS
#  define ECS_MAX_SYSTEMS 1024
# endif

/*
** Component is a hash table containing key per entity
*/

typedef t_hash_table*	t_component;
typedef t_hash_table*	t_type_mask;

typedef void			(*t_system_handle)(t_world *world);

/*
** Components mask holds information on which components belong to the system
** or which indices from world component_list belong to the system.
*/

typedef struct			s_system
{
	const char			*name;
	uint64_t			system_index;
	uint64_t			components_mask;
	t_system_handle		system_handle_func;
}						t_system;

/*
** World is the core of the ECS library, it contains entities, and maps
** relations between entities and components they hold. It also contains
** a reference to all possible components that entities have.
*/

typedef struct			s_world
{
	uint64_t			max_entities;
	uint64_t			*entity_masks;
	t_entities			entities;
	uint64_t			next_free_entity_index;
	int64_t				current_entity_index;
	t_type_mask			*type_masks[ECS_MAX_COMPONENTS];
	uint64_t			type_ids[ECS_MAX_ENTITY_TYPES];
	t_component			component_list[ECS_MAX_COMPONENTS];
	t_system			systems[MAX_SYSTEMS];
	uint64_t			next_free_system_index;
}						t_world;

/*
** World init & destroy
*/

t_world					*world_create(const char *name, uint64_t max_entities);
t_world					*world_destroy(t_world *world);

/*
** Components
*/

void					world_component_add(t_world *world,
						t_component *parts);
void					world_component_remove(t_world *world,
						t_component *parts);

/*
** Entities
*/

uint64_t				world_entity_add(t_world *world, uint64_t components);
void					world_entity_component_add(t_world *world,
						uint64_t entity, uint64_t component);
void					world_entity_remove(t_world *world, uint64_t entity);
void					world_entity_component_remove(t_world *world,
						uint64_t entity, uint64_t component);
bool					world_entity_valid(t_world *world,
						uint64_t entity);
bool					world_entity_contains(t_world *world, uint64_t entity,
						uint64_t components);

/*
** Systems
*/

void					world_system_add(t_world *world, t_system system);
void					world_system_remove(t_world *world, t_system system);

/*
** ECS utils
*/

#endif
