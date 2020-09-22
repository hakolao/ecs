/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   demo.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 17:46:27 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/22 13:25:40 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEMO_H
# define DEMO_H

# include <SDL.h>
# include <SDL_image.h>
# include <SDL_ttf.h>
# include "libft.h"
# include "libecs.h"

# define NUM_THREADS 8

# define WIDTH 1280
# define HEIGHT 720

# define NAME "Demo World"
# define PIXEL_FORMAT SDL_PIXELFORMAT_RGBA8888
# define FONT "demo/assets/pixelated.ttf"
# define FONT_SIZE 20

# define MAX_ENTITIES 10000
# define ENTITY_SIZE 30

# define CONST_GRAVITY 9.81
# define CONST_MOVEMENT 1.0

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
	t_bool					is_gravity;
}							t_app;

/*
** Demo components (to be used by each entity)
*/

typedef struct				s_pos
{
	float	x;
	float	y;
}							t_pos;

typedef struct				s_vel
{
	float	dx;
	float	dy;
}							t_vel;

typedef struct				s_physics
{
	float	mass;
	float	inertia;
	t_pos	position;
	t_vel	velocity;
}							t_physics;

typedef struct				t_visuals
{
	uint32_t	color;
	uint32_t	width;
	uint32_t	height;
	uint32_t	z_val;
}							t_visuals;

/*
** Component identifiers, should be powers of 2 and ULL for valid component
** ids.
*/

typedef enum				e_comp_id
{
	comp_empty = 0ULL,
	comp_physics = 1ULL,
	comp_vis = 1ULL << 1,
}							t_comp_id;

/*
** System ids.
** If a system should not be run with other systems, its id should not be 0
*/

typedef enum				e_system_id
{
	system_render = 123,
	system_zbuffer = 1515,
	system_forces = 111,
	system_reset = 222,
}							t_system_id;

/*
** System
*/
void						systems_create(t_app *app);
void						systems_params_update(t_app *app);

/*
** Entities
*/
void						entities_create_up_to_max(t_app *app);
void						init_entity(t_app *app, t_physics *physics,
							t_visuals *visuals);

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
