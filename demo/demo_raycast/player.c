/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/24 14:09:55 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/25 18:50:45 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "demo_raycast.h"

void			apply_transforms_to_world(t_demo_data *data)
{
	int		i;

	i = -1;
	while (++i < (int)data->num_objects)
	{
		transform_3d_object(data->objects[i], data->world_scale);
		transform_3d_object(data->objects[i], data->world_rotation);
		transform_3d_object(data->objects[i], data->world_translation);
		update_3d_object_kd_tree(data->objects[i]);
	}
}

void			rotate_player(t_demo_data *data, uint32_t delta_time, t_vec3 axes)
{
	t_mat4	rotation;

	ml_vector3_mul(axes, ml_rad(data->player_rot_speed * delta_time), axes);
	ml_matrix4_rotation(-axes[0], -axes[1], -axes[2], rotation);
	ml_matrix4_mul_vec3(rotation, data->player_forward, data->player_forward);
	ml_matrix4_mul_vec3(rotation, data->player_up, data->player_up);
	ml_matrix4_mul_vec3(rotation, data->player_sideways, data->player_sideways);
}

void			move_player(t_demo_data *data, uint32_t delta_time, t_move dir)
{
	t_vec3		add;
	t_vec3		new_pos;
	t_mat4		translation;

	if (dir == move_forward)
	{
		ml_vector3_mul(data->player_forward, -data->player_speed * delta_time, add);
		ml_vector3_sub(data->player_pos, add, new_pos);
	}
	else if (dir == move_backward)
	{
		ml_vector3_mul(data->player_forward, data->player_speed * delta_time, add);
		ml_vector3_sub(data->player_pos, add, new_pos);
	}
	else if (dir == move_strafe_left)
	{
		ml_vector3_mul(data->player_sideways, -data->player_speed * delta_time, add);
		ml_vector3_sub(data->player_pos, data->player_sideways, new_pos);
	}
	else if (dir == move_strafe_right)
	{
		ml_vector3_mul(data->player_sideways, data->player_speed * delta_time, add);
		ml_vector3_sub(data->player_pos, data->player_sideways, new_pos);
	}
	ml_vector3_copy(new_pos, data->player_pos);
	ml_matrix4_translation(-add[0], -add[1], -add[2], translation);
}
