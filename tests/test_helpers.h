/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_helpers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/16 17:23:16 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/16 17:40:52 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_HELPERS_H
# define TEST_HELPERS_H

typedef struct		s_position
{
	int			x;
	int			y;
}					t_position;

typedef struct		s_velocity
{
	float			x;
	float			y;
}					t_velocity;

typedef enum		e_component_id
{
	component_empty = 0,
	component_position = 1,
	component_velocity = 1 << 1,
}					t_component_id;

#endif