/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   demo_raycast.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 17:46:27 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/22 17:25:56 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEMO_RAYCAST_H
# define DEMO_RAYCAST_H

# include "demo_common.h"

# define NUM_THREADS 8

# define NAME "Demo Raycast"

# define NUM_TRIANGLES 5

typedef struct				s_data
{
	uint32_t				num_triangles;
}							t_data;

/*
** Component identifiers, should be powers of 2 and ULL for valid component
** ids.
*/

typedef enum				e_comp_id
{
	comp_empty = 0ULL,
	comp_triangle = 1ULL,
	comp_ray = 1ULL << 1,
}							t_comp_id;

/*
** System ids.
** If a system should not be run with other systems, its id should not be 0
*/

typedef enum				e_system_id
{
	system_render = 111,
	system_zbuffer = 222,
}							t_system_id;

/*
** System
*/
void						systems_create(t_app *app);
void						systems_params_update(t_app *app);

/*
** Entities
*/
//ToDo

#endif
