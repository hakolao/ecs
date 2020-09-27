/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/24 14:09:55 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/27 18:16:44 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "demo_raycast.h"

void			update_world_transform(t_demo_data *data)
{
	int				i;
	t_mat4			tmp;
	t_mat4			transform;
	t_mat4			inverse;

	ml_matrix4_inverse(data->world_transform, inverse);
	ml_matrix4_mul(data->world_translation, data->world_scale, tmp);
	ml_matrix4_mul(data->world_rotation, tmp, transform);
	i = -1;
	while (++i < (int)data->num_objects)
	{
		transform_3d_object(data->objects[i], inverse);
		transform_3d_object(data->objects[i], transform);
		update_3d_object_kd_tree(data->objects[i]);
	}
	ft_memcpy(data->world_transform, transform, sizeof(t_mat4));
}

void			rotate_player(t_demo_data *data, uint32_t delta_time, t_vec3 axes)
{
	t_mat4	rotation;
	t_mat4	new_world_rotation;

	ml_vector3_mul(axes, ml_rad(data->player_rot_speed * delta_time), axes);
	ml_matrix4_rotation(-axes[0], -axes[1], -axes[2], rotation);
	ml_matrix4_mul(data->world_rotation, rotation, new_world_rotation);
	ft_memcpy(data->world_rotation, new_world_rotation, sizeof(t_mat4));
	update_world_transform(data);
}

void			move_player(t_demo_data *data, uint32_t delta_time, t_move dir)
{
	t_vec3		add;
	t_vec3		new_pos;
	t_mat4		translation;
	t_mat4		new_world_translation;

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
	ml_matrix4_mul(data->world_translation, translation, new_world_translation);
	ft_memcpy(data->world_translation, new_world_translation, sizeof(t_mat4));
	update_world_transform(data);
}
