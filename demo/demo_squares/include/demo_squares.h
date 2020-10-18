/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   demo_squares.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 17:46:27 by ohakola           #+#    #+#             */
/*   Updated: 2020/10/18 22:05:07 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEMO_SQUARES_H
# define DEMO_SQUARES_H

# include "demo_common.h"

# define NAME "Demo World"

# define MAX_ENTITIES 10000
# define ENTITY_SIZE 30

# define CONST_GRAVITY 9.81
# define CONST_MOVEMENT 1.0

/*
** Demo components (to be used by each entity)
*/

typedef struct				s_pos
{
	float	x;
	float	y;
}							t_pos;

typedef struct				s_vel
{
	float	dx;
	float	dy;
}							t_vel;

typedef struct				s_physics
{
	float	mass;
	float	inertia;
	t_pos	position;
	t_vel	velocity;
}							t_physics;

typedef struct				t_visuals
{
	uint32_t	color;
	uint32_t	width;
	uint32_t	height;
	uint32_t	z_val;
}							t_visuals;

/*
** Component identifiers, should be powers of 2 and ULL for valid component
** ids.
*/

typedef enum				e_comp_id
{
	comp_empty = 0ULL,
	comp_physics = 1ULL,
	comp_vis = 1ULL << 1,
}							t_comp_id;

/*
** System ids.
** If a system should not be run with other systems, its id should not be 0
*/

typedef enum				e_system_id
{
	system_render = 123,
	system_zbuffer = 1515,
	system_forces = 111,
	system_reset = 222,
}							t_system_id;

/*
** System
*/
void						systems_create(t_app *app);
void						systems_params_update(t_app *app);

/*
** Entities
*/
void						entities_create_up_to_max(t_app *app);
void						init_entity(t_app *app, t_physics *physics,
							t_visuals *visuals);


#endif
