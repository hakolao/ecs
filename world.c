/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/12 15:30:39 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/18 14:21:16 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libecs.h"

/*
** Creates an entity component system world initializing all arrays and
** hash_maps with given parameters (max_entities)
*/

t_ecs_world			*ecs_world_create(const char *name, uint64_t max_entities)
{
	t_ecs_world	*world;

	if (!(world = malloc(sizeof(*world))) ||
		!(world->name = ft_strdup(name)) ||
		!(world->entities = malloc(sizeof(uint64_t) * max_entities)) ||
		!(world->vacant_entities = malloc(sizeof(uint64_t) * max_entities)))
		return (NULL);
	ft_memset(world->vacant_entities, -1, sizeof(uint64_t) * max_entities);
	world->max_entities = max_entities;
	world->num_entities = 0;
	world->next_free_entity_index = 0;
	ft_memset(world->components_by_entity, 0,
		sizeof(t_hash_table*) * ECS_MAX_COMPONENTS);
	world->next_free_component_index = 0;
	world->index_by_component = hash_map_create(ECS_MAX_COMPONENTS);
	world->num_components = 0;
	ft_memset(world->systems, 0, sizeof(uint64_t) * ECS_MAX_SYSTEMS);
	world->num_systems = 0;
	world->next_free_system_index = 0;
	world->index_by_system = hash_map_create(ECS_MAX_SYSTEMS);
	world->next_vacancy_index = -1;
	return (world);
}

/*
** Destroys the world clearing its content from memory.
*/

void			ecs_world_destroy(t_ecs_world *world)
{
	int	i;
	int	removed;

	ft_strdel(&world->name);
	free(world->entities);
	free(world->vacant_entities);
	hash_map_destroy(world->index_by_component);
	hash_map_destroy(world->index_by_system);
	i = -1;
	removed = 0;
	while (++i < (int)world->num_components + removed)
	{
		if (world->components_by_entity[i] != NULL)
			hash_map_destroy_free(world->components_by_entity[i]);
		else
			removed++;
	}
	free(world);
	world = NULL;
}
