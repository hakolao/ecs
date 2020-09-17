/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libecs.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/17 13:11:01 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/17 13:17:10 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBECS_H
# define LIBECS_H

# include "libft.h"
# include <stdarg.h>

/*
** 64 (max number of bits in uint64_t (for bitmasks)). Might be unnecessary
** limitation...
*/

# define ECS_MAX_COMPONENTS 64

/*
** Redefine these if you wish more capacity for entity types & systems
*/

#  define ECS_MAX_ENTITY_TYPES 1024
#  define ECS_MAX_SYSTEMS 1024

# define ECS_SYSTEM_EMPTY UINT64_MAX

# define ECS_NO_COMPONENT 0

# define ECS_EMPTY_ENTITY 0

typedef struct s_world	t_world;

typedef void			(*t_system_handle)(t_world *world);

/*
** Components mask holds information on which components belong to the system
** or which indices from world component_list belong to the system.
*/

typedef struct			s_system
{
	uint64_t			system_id;
	uint64_t			components_mask;
	t_system_handle		system_handle_func;
}						t_system;

typedef struct			s_component
{
	uint64_t			id;
	size_t				size;
	void				*data;
}						t_component;

/*
** World is the core of the ECS library, it contains entities, and maps
** relations between entities and components they hold. It also contains
** a reference to all possible components that entities have.
*/

struct			s_world
{
	char				*name;
	uint64_t			max_entities;
	uint64_t			num_entities;
	uint64_t			*entities;
	int64_t				*vacant_entities;
	uint64_t			next_free_entity_index;
	int64_t				next_vacant_entity_index;
	t_hash_table		*component_list[ECS_MAX_COMPONENTS];
	uint64_t			next_free_component_index;
	t_hash_table		*component_to_list;
	uint64_t			num_components;
	t_system			systems[ECS_MAX_SYSTEMS];
	uint64_t			next_free_system_index;
	uint64_t			num_systems;
};

/*
** World init & destroy
*/

t_world					*world_create(const char *name, uint64_t max_entities);
void					world_destroy(t_world *world);

/*
** Components
*/

void					world_component_add(t_world *world, uint64_t component);
void					world_component_remove(t_world *world,
						uint64_t component);

/*
** Entities
*/

int64_t					world_entity_add(t_world *world,
						uint64_t num_components, ...);
void					world_entity_remove(t_world *world,
						uint64_t entity_index);
uint64_t				world_entity_get(t_world *world,
						uint64_t entity_index);
void					world_entity_components_add(t_world *world,
						uint64_t entity_index, uint64_t num_components, ...);
void					world_entity_component_remove(t_world *world,
						uint64_t entity_index, uint64_t component);
t_bool					world_entity_valid(t_world *world,
						uint64_t entity_index);
t_bool					world_entity_contains(t_world *world,
						uint64_t entity_index, uint64_t components);
void					*world_entity_component_get(t_world *world,
						uint64_t entity_index, uint64_t component);

/*
** Systems
*/

void					world_system_add(t_world *world, t_system system);
void					world_system_remove(t_world *world, uint64_t system_id);

/*
** ECS utils
*/

#endif
