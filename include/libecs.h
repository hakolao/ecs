/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libecs.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/17 13:11:01 by ohakola           #+#    #+#             */
/*   Updated: 2020/10/18 22:25:13 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBECS_H
# define LIBECS_H

# include "libft.h"
# include <stdarg.h>

/*
** 64 (max number of bits in uint64_t (for bitmasks)).
** Entities are like 11110001111, where each bit corresponds to one component.
** Thus a component should be defined like 1 << 3 as an enum. See demo example
** or tests.
*/

# define ECS_MAX_COMPONENTS 64

/*
** Redefine these if you wish more capacity for entity types & systems
*/

# define ECS_MAX_ENTITY_TYPES 1024
# define ECS_MAX_SYSTEMS 1024

/*
** Some util macros
*/

# define ECS_SYSTEM_EMPTY UINT32_MAX
# define ECS_NO_COMPONENT 0
# define ECS_EMPTY_ENTITY 0

typedef struct s_ecs_world	t_ecs_world;

typedef void				(*t_system_handle)(t_ecs_world *world,
							uint64_t entity_index);

/*
** Components mask holds information on which components belong to the system
** or which indices from world component_list belong to the system.
*/

typedef struct				s_system
{
	uint64_t			system_id;
	uint64_t			components_mask;
	t_system_handle		system_handle_func;
	void				*params;
}							t_system;

/*
** Component data struct to hold arbitrary data that gets passed to hash maps
** which return component by entity index. Size is necessary so we know
** how much to allocate and copy for entity.
*/

typedef struct				s_component
{
	uint64_t			id;
	size_t				size;
	void				*data;
}							t_component;

/*
** World is the core of the ECS library, it contains entities, and maps
** relations between entities and components they hold. It also contains
** a reference to all possible components that entities have.
** @entities: Enity masks (set of components) per entity
** @vacant_entities: Holds knowledge of removed entities and those indices
** are filled first when adding new entities.
** @components_by_entity: A list of hash maps per each component holding
** component data by each entity index.
** @index_by_component: Hash map containing an index per component to access
** components_by_entity faster.
** @systems: Holds system data and info on components belonging to that system
** this is used to run system handles on each entity that matches the components
** @index_by_system: A hash map holding the indices by system id to faster
** access systems.
*/

struct						s_ecs_world
{
	char				*name;
	uint64_t			max_entities;
	uint64_t			num_entities;
	uint64_t			*entities;
	int64_t				*vacant_entities;
	uint64_t			next_free_entity_index;
	int64_t				next_vacancy_index;
	t_hash_table		*components_by_entity[ECS_MAX_COMPONENTS];
	uint64_t			next_free_component_index;
	t_hash_table		*index_by_component;
	uint64_t			num_components;
	t_system			systems[ECS_MAX_SYSTEMS];
	uint64_t			next_free_system_index;
	t_hash_table		*index_by_system;
	uint64_t			num_systems;
};

typedef struct				s_entity_id_group
{
	uint64_t			*ids;
	uint32_t			num_ids;
}							t_entity_id_group;

typedef struct				s_system_parallel
{
	t_ecs_world			**world;
	uint32_t			thread_id;
	uint32_t			num_threads;
	uint64_t			systems;
	uint32_t			min_entity_index;
	uint32_t			max_entity_index;
	t_entity_id_group	*entity_group;
}							t_system_parallel;

/*
** World creation & destroy
*/

t_ecs_world					*ecs_world_create(const char *name,
							uint64_t max_entities);
void						ecs_world_destroy(t_ecs_world *world);

/*
** Components related to world data
*/

void						ecs_world_component_add(t_ecs_world *world,
							uint64_t component);
void						ecs_world_component_remove(t_ecs_world *world,
							uint64_t component);
uint64_t					ecs_component_index(t_ecs_world *world,
							uint64_t component_id);
t_hash_table				*ecs_component_entities(t_ecs_world *world,
							uint64_t component_id);

/*
** Entities & entity management
*/

int64_t						ecs_world_entity_add(t_ecs_world *world,
							uint64_t num_components, ...);
void						ecs_world_entity_remove(t_ecs_world *world,
							uint64_t entity_index);
uint64_t					ecs_world_entity_get(t_ecs_world *world,
							uint64_t entity_index);
void						ecs_world_entity_components_add(t_ecs_world *world,
							uint64_t entity_index,
							uint64_t num_components, ...);
void						ecs_world_entity_components_remove(t_ecs_world
							*world, uint64_t entity_index,
							uint64_t components_to_remove);
t_bool						ecs_world_entity_valid(t_ecs_world *world,
							uint64_t entity_index);
t_bool						ecs_world_entity_at_contains(t_ecs_world *world,
							uint64_t entity_index, uint64_t components);
t_bool						ecs_world_entity_contains(uint64_t entity,
							uint64_t components);
void						*ecs_world_entity_component_get(t_ecs_world *world,
							uint64_t entity_index, uint64_t component);

/*
** Systems and system management
*/

void						ecs_world_system_add(t_ecs_world *world,
							t_system system);
void						ecs_world_system_remove(t_ecs_world *world,
							uint64_t system_id);
uint64_t					ecs_system_index(t_ecs_world *world,
							uint64_t system_id);
void						ecs_systems_run(t_ecs_world *world,
							uint64_t systems);
void						ecs_system_update_params(t_ecs_world *world,
							uint64_t system_id, void *params);
void						ecs_systems_run_single(t_ecs_world *world,
							uint64_t system_id);
void						ecs_systems_run_parallel(t_thread_pool *thread_pool,
							t_ecs_world *world, uint64_t systems);
t_system					ecs_world_system_get(t_ecs_world *world,
							uint64_t system_id);
void						ecs_systems_run_parallel_on_entities(t_thread_pool
							*thread_pool, t_ecs_world *world, uint64_t systems,
							t_hash_table *entities_by_thread);

#endif
