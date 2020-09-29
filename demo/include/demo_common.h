/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   demo_common.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 17:10:31 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/29 15:16:49 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEMO_COMMON_H
# define DEMO_COMMON_H

# include <SDL.h>
# include <SDL_image.h>
# include <SDL_ttf.h>
# include "libft.h"
# include "libecs.h"

# define PIXEL_FORMAT SDL_PIXELFORMAT_RGBA8888
# define FONT "demo/assets/pixelated.ttf"
# define FONT_SIZE 20

# define WIDTH 640
# define HEIGHT 360

typedef struct				s_text_params
{
	const char				*text;
	SDL_Color				text_color;
	int						*xy;
	float					blend_ratio;
}							t_text_params;

typedef struct				s_window
{
	SDL_Renderer			*renderer;
	SDL_Texture				*frame;
	uint32_t				*framebuffer;
	uint32_t				*zbuffer;
	TTF_Font				*font;
	int32_t					width;
	int32_t					height;
	int32_t					pitch;
	t_bool					resized;
	SDL_Window				*window;
	uint32_t				window_id;
	t_bool					is_hidden;
}							t_window;

typedef struct				s_info
{
	uint32_t				fps;
	uint64_t				delta_time;
	uint64_t				performance_start;
	uint64_t				performance_end;
}							t_info;

typedef struct				s_app
{
	t_window				*window;
	t_info					info;
	t_ecs_world				*world;
	void					*data;
}							t_app;

/*
** Window
*/

void						window_init(t_app *app);
void						recreate_frame(t_app *app);

/*
** Colors
*/
uint32_t					rgba_to_u32(SDL_Color color);
uint32_t					color_blend_u32(uint32_t color1,
							uint32_t color2, float ratio);
SDL_Color					u32_to_rgba(uint32_t color);

/*
** Text
*/

void						render_text(t_app *app, t_text_params params);

/*
** Utils
*/

void						draw_fps(t_app *app);
uint32_t					capture_framerate(uint32_t time_since_start);

#endif
