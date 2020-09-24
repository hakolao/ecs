/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 17:54:10 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/24 17:25:02 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "demo_common.h"

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
	char	*dt_str;

	if (!(fps_str = ft_itoa_base_u32(app->info.fps, 10)) ||
		!(dt_str = ft_itoa_base_u32(app->info.delta_time, 10)))
		return ;
	render_text(app, (t_text_params){
		.text = fps_str, .blend_ratio = 1.0,
		.xy = (int[2]){5, 5},
		.text_color = (SDL_Color){255, 0, 0, 255}});
	render_text(app, (t_text_params){
		.text = dt_str, .blend_ratio = 1.0,
		.xy = (int[2]){5, 5 + FONT_SIZE + 5},
		.text_color = (SDL_Color){255, 0, 0, 255}});
	ft_strdel(&fps_str);
	ft_strdel(&dt_str);
}
