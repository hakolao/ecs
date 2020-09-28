/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/24 14:09:55 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/28 14:54:34 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "demo_raycast.h"

void			update_world_transform(t_scene *scene)
{
	int				i;
	t_mat4			tmp;
	t_mat4			transform;
	t_mat4			inverse;

	ml_matrix4_inverse(scene->world_transform, inverse);
	ml_matrix4_mul(scene->world_scale, scene->world_rotation, tmp);
	ml_matrix4_mul(scene->world_translation, tmp, transform);
	i = -1;
	while (++i < (int)scene->num_objects)
	{
		transform_3d_object(scene->objects[i], inverse);
		transform_3d_object(scene->objects[i], transform);
	}
	ft_memcpy(scene->world_transform, transform, sizeof(t_mat4));
	update_scene_triangle_tree(scene);
}

void			rotate_player(t_scene *scene, uint32_t delta_time, t_vec3 axes)
{
	t_mat4	rotation;
	t_mat4	new_world_rotation;

	ml_vector3_mul(axes, ml_rad(scene->player_rot_speed * delta_time), axes);
	ml_matrix4_rotation(-axes[0], -axes[1], -axes[2], rotation);
	ml_matrix4_mul_vec3(rotation, scene->player_forward, scene->player_forward);
	ml_matrix4_mul_vec3(rotation, scene->player_up, scene->player_up);
	ml_matrix4_mul_vec3(rotation, scene->player_sideways, scene->player_sideways);
	ml_matrix4_mul(scene->world_rotation, rotation, new_world_rotation);
	ft_memcpy(scene->world_rotation, new_world_rotation, sizeof(t_mat4));
	update_world_transform(scene);
}

void			move_player(t_scene *scene, uint32_t delta_time, t_move dir)
{
	t_vec3		add;
	t_vec3		new_pos;
	t_mat4		translation;
	t_mat4		new_world_translation;

	if (dir == move_forward)
	{
		ml_vector3_mul(scene->player_forward, -scene->player_speed * delta_time, add);
		ml_vector3_sub(scene->player_pos, add, new_pos);
	}
	else if (dir == move_backward)
	{
		ml_vector3_mul(scene->player_forward, scene->player_speed * delta_time, add);
		ml_vector3_sub(scene->player_pos, add, new_pos);
	}
	else if (dir == move_strafe_left)
	{
		ml_vector3_mul(scene->player_sideways, -scene->player_speed * delta_time, add);
		ml_vector3_sub(scene->player_pos, scene->player_sideways, new_pos);
	}
	else if (dir == move_strafe_right)
	{
		ml_vector3_mul(scene->player_sideways, scene->player_speed * delta_time, add);
		ml_vector3_sub(scene->player_pos, scene->player_sideways, new_pos);
	}
	ml_vector3_copy(new_pos, scene->player_pos);
	ml_matrix4_translation(-add[0], -add[1], -add[2], translation);
	ml_matrix4_mul(scene->world_translation, translation, new_world_translation);
	ft_memcpy(scene->world_translation, new_world_translation, sizeof(t_mat4));
	update_world_transform(scene);
}
