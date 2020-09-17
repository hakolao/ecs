/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   demo.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 17:46:27 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/17 18:54:48 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEMO_H
# define DEMO_H

# include <SDL.h>
# include <SDL_image.h>
# include <SDL_ttf.h>
# include "libft.h"
# include "libecs.h"

# define WIDTH 1280
# define HEIGHT 720

# define PIXEL_FORMAT SDL_PIXELFORMAT_RGBA8888
# define FONT "demo/assets/pixelated.ttf"
# define FONT_SIZE 20

typedef struct				s_text_params
{
	const char				*font;
	const char				*text;
	SDL_Color				text_color;
	int						font_size;
	int						*xy;
	float					blend_ratio;
}							t_text_params;

typedef struct				s_window
{
	SDL_Renderer			*renderer;
	SDL_Texture				*frame;
	uint32_t				*framebuffer;
	TTF_Font				*font;
	int32_t					width;
	int32_t					height;
	int32_t					pitch;
	t_bool					resized;
	SDL_Window				*window;
	uint32_t				window_id;
	t_bool					is_hidden;
	void					*parent;
}							t_window;

typedef struct				s_app
{
	t_window				*window;
	uint32_t				fps;
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
void						error_check(int test, const char *message);
void						draw_fps(t_app *app);
uint32_t					capture_framerate(uint32_t time_since_start);
uint32_t					get_relative_font_size(t_app *app,
							uint32_t font_size);

#endif
