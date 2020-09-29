/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   l3d_obj_read_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 15:35:00 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/29 16:02:05 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib3d.h"

static t_bool			l3d_is_valid_obj(t_obj *obj)
{
	int				i;
	int				j;
	uint32_t		max_v_index;
	uint32_t		max_vt_index;
	uint32_t		max_vn_index;

	i = -1;
	max_v_index = 0;
	max_vt_index = 0;
	max_vn_index = 0;
	while (++i < (int)obj->num_triangles)
	{
		j = -1;
		while (++j < 3)
		{
			max_v_index = max_v_index > obj->triangles[i * 9 + j * 3 + 0] ?
				max_v_index : obj->triangles[i * 9 + j * 3 + 0];
			max_vt_index = max_vt_index > obj->triangles[i * 9 + j * 3 + 1] ?
				max_vt_index : obj->triangles[i * 9 + j * 3 + 1];
			max_vn_index = max_vn_index > obj->triangles[i * 9 + j * 3 + 2] ?
				max_vn_index : obj->triangles[i * 9 + j * 3 + 2];
		}
	}
	return (max_v_index == obj->num_vertices &&
			max_vt_index == obj->num_v_text_coords &&
			max_vn_index == obj->num_v_normals );
}

void					l3d_read_obj_vec3_line(char **str, t_vec3 res)
{
	char	*tmp;

	tmp = *str;
	while (*tmp &&
		(ft_isdigit(*tmp) || *tmp == ' ' || *tmp == '-' || *tmp == '.'))
		tmp++;
	if (*tmp != '\n')
		error_check(true, "Invalid characters in vec3 data (v)");
	res[0] = ft_atod(*str);
	ft_scroll_over(str, ' ');
	res[1] = ft_atod(*str);
	ft_scroll_over(str, ' ');
	res[2] = ft_atod(*str);
	ft_scroll_over(str, '\n');
}

void					l3d_read_obj_vec2_line(char **str, t_vec3 res)
{
	char	*tmp;

	tmp = *str;
	while (*tmp &&
		(ft_isdigit(*tmp) || *tmp == ' ' || *tmp == '-' || *tmp == '.'))
		tmp++;
	if (*tmp != '\n')
		error_check(true, "Invalid characters in vec2 data (vn || vt)");
	res[0] = ft_atod(*str);
	ft_scroll_over(str, ' ');
	res[1] = ft_atod(*str);
	ft_scroll_over(str, '\n');
}

void					l3d_read_obj_triangle_line(char **str, uint32_t *triangle)
{
	char	*tmp;
	int		i;

	tmp = *str;
	while (*tmp && (*tmp == '/' || ft_isdigit(*tmp) || *tmp == ' '))
		tmp++;
	if (*tmp != '\n')
		error_check(true, "Invalid characters in triangle data");
	i = -1;
	while (++i < 9)
	{
		*(triangle + i) = ft_atoi(*str);
		if (i == 2 || i == 5)
			ft_scroll_over(str, ' ');
		else if (i < 8)
			ft_scroll_over(str, '/');
	}
	tmp = *str;
	while (ft_isdigit(*tmp))
		tmp++;
	if (*tmp != '\n')
		error_check(true, "Invalid triangle, read_obj expect faces to be "
			"triangles e.g. (1/1/1/ 2/2/2 3/3/3\n");
	ft_scroll_over(str, '\n');
}

void					l3d_obj_str_parse(char *str, t_obj_content *result)
{
	t_obj	*o;

	ft_memset(result, 0, sizeof(*result));
	while (*str)
	{
		while (*str != 'o' && *(str + 1) != ' ')
			str++;
		if (*str)
		{
			o = &result->objects[result->num_objects];
			allocate_single_obj(o);
			while (*str)
			{
				if (*str == 'v' && *(str + 1) == ' ' && (str += 2))
					l3d_read_obj_vec3_line(&str, o->v[o->num_vertices++]);
				else if (*str == 'v' && *(str + 1) == 't' && *(str + 2) == ' ' && (str += 3))
					l3d_read_obj_vec2_line(&str, o->vt[o->num_v_text_coords++]);
				else if (*str == 'v' && *(str + 1) == 'n' && *(str + 2) == ' ' && (str += 3))
					l3d_read_obj_vec3_line(&str, o->vn[o->num_v_normals++]);
				else if (*str == 'f' && *(str + 1) == ' ' && (str += 2))
				{
					l3d_read_obj_triangle_line(&str, o->triangles + o->num_triangles * 9);
					o->num_triangles++;
				}
				else
					str++;
			}
			result->num_objects++;
		}
	}
}
