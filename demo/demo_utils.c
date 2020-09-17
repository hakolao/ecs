/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   demo_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 17:54:10 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/17 19:08:46 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "demo.h"

void				error_check(int test, const char *message)
{
	if (test)
	{
		ft_dprintf(2, "%s\n", message);
		exit(1);
	}
}

uint32_t			capture_framerate(uint32_t delta_time)
{
	static uint32_t		delta_time_sum;
	static uint32_t		frames_per_sec;
	static uint32_t		prev_fps;

	delta_time_sum += delta_time;
	frames_per_sec++;
	if (delta_time_sum > 1000.0)
	{
		prev_fps = frames_per_sec;
		delta_time_sum = 0;
		frames_per_sec = 0;
		return (frames_per_sec);
	}
	return (prev_fps);
}

void				draw_fps(t_app *app)
{
	char	*fps_str;

	if (!(fps_str = ft_itoa_base_u32(app->fps, 10)))
		return ;
	render_text(app, (t_text_params){
		.text = fps_str, .font = FONT, .blend_ratio = 1.0,
		.font_size = FONT_SIZE, .xy = (int[2]){5, 5},
		.text_color = (SDL_Color){255, 0, 0, 255}});
	ft_strdel(&fps_str);
}

uint32_t			get_relative_font_size(t_app *app, uint32_t font_size)
{
	float		aspect_ratio;

	aspect_ratio = (float)app->window->width /
					(float)app->window->height;
	return (app->window->width / (float)WIDTH * aspect_ratio * font_size);
}
