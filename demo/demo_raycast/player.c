/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/24 14:09:55 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/25 16:17:45 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "demo_raycast.h"

static void		apply_transform_to_world(t_demo_data *data, t_mat4 transform)
{
	int		i;

	i = -1;
	while (++i < (int)data->num_objects)
		transform_3d_object(data->objects[i], transform);
}

/*
** !Note: Diff axes have been switched to accommodate our chosen axes for the
** world & camera.
*/

static void		apply_movement(t_demo_data *data, t_vec3 new_pos)
{
	ml_vector3_copy(new_pos, data->player_pos);
}

void			rotate_player(t_demo_data *data, uint32_t delta_time, t_vec3 axes)
{
	t_mat4	rotation;

	ml_vector3_mul(axes, ml_rad(data->player_rot_speed * delta_time), axes);
	ml_matrix4_rotation(-axes[0], -axes[1], -axes[2], rotation);
	ml_matrix4_mul_vec3(rotation, data->player_forward, data->player_forward);
	ml_matrix4_mul_vec3(rotation, data->player_up, data->player_up);
	ml_matrix4_mul_vec3(rotation, data->player_sideways, data->player_sideways);
	apply_transform_to_world(data, rotation);
}

void			move_player(t_demo_data *data, uint32_t delta_time, t_move dir)
{
	t_vec3		add;
	t_vec3		new_pos;

	if (dir == move_forward)
	{
		ml_vector3_mul(data->player_forward, data->player_speed * delta_time, add);
		ml_vector3_sub(data->player_pos, add, new_pos);
	}
	else if (dir == move_backward)
	{
		ml_vector3_mul(data->player_forward, data->player_speed * delta_time, add);
		ml_vector3_add(data->player_pos, add, new_pos);
	}
	else if (dir == move_strafe_left)
	{
		ml_vector3_mul(data->player_sideways, data->player_speed * delta_time, add);
		ml_vector3_add(data->player_pos, data->player_sideways, new_pos);
	}
	else if (dir == move_strafe_right)
	{
		ml_vector3_mul(data->player_sideways, data->player_speed * delta_time, add);
		ml_vector3_sub(data->player_pos, data->player_sideways, new_pos);
	}
	apply_movement(data, new_pos);
}
