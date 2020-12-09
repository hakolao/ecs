/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 18:09:20 by ohakola           #+#    #+#             */
/*   Updated: 2020/12/09 18:57:14 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "demo_common.h"

static SDL_Surface	*surface_from_font(t_app *app, t_text_params params)
{
	SDL_Surface		*text_surface;
	SDL_Surface		*formatted_surface;

	text_surface = TTF_RenderText_Blended_Wrapped(
		app->window->font, params.text, params.text_color, app->window->width);
	error_check(!text_surface, TTF_GetError());
	formatted_surface = SDL_ConvertSurfaceFormat(text_surface,
		PIXEL_FORMAT, 0);
	SDL_FreeSurface(text_surface);
	error_check(!formatted_surface, SDL_GetError());
	return (formatted_surface);
}

void				render_text(t_app *app, t_text_params params)
{
	SDL_Surface	*surface;

	surface = surface_from_font(app, params);
	l3d_image_place(
		&(t_surface){.h = app->window->height,
			.w = app->window->width,
			.pixels = app->window->framebuffer},
		&(t_surface){.h = surface->h,
			.w = surface->w,
			.pixels = surface->pixels},
		params.xy, params.blend_ratio);
	SDL_FreeSurface(surface);
}
