/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 18:00:29 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/30 01:34:17 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "demo_common.h"

static int		resize_callback(void *data, SDL_Event *event)
{
	t_app	*app;

	if (event->type == SDL_WINDOWEVENT &&
		(event->window.event == SDL_WINDOWEVENT_RESIZED ||
		event->window.event == SDL_WINDOWEVENT_MINIMIZED ||
		event->window.event == SDL_WINDOWEVENT_MAXIMIZED ||
		event->window.event == SDL_WINDOWEVENT_SHOWN ||
		event->window.event == SDL_WINDOWEVENT_HIDDEN))
	{
		app = (t_app*)data;
		if (event->window.windowID == app->window->window_id)
		{
			app->window->resized = true;
			SDL_GetWindowSize(app->window->window, &app->window->width,
				&app->window->height);
			if (event->window.event == SDL_WINDOWEVENT_HIDDEN)
				app->window->is_hidden = true;
			else if (event->window.event == SDL_WINDOWEVENT_SHOWN)
				app->window->is_hidden = false;
		}
	}
	return (0);
}

void			recreate_frame(t_app *app)
{
	if (app->window->frame != NULL)
		SDL_DestroyTexture(app->window->frame);
	app->window->frame = SDL_CreateTexture(app->window->renderer,
		PIXEL_FORMAT, SDL_TEXTUREACCESS_STREAMING, app->window->width,
		app->window->height);
	error_check(app->window->frame == NULL, SDL_GetError());
	if (app->window->framebuffer != NULL)
		free(app->window->framebuffer);
	error_check(!(app->window->framebuffer = malloc(sizeof(uint32_t) *
		app->window->width * app->window->height)),
		"Failed to malloc framebuffer in resize");
	if (app->window->zbuffer != NULL)
		free(app->window->zbuffer);
	error_check(!(app->window->zbuffer = malloc(sizeof(uint32_t) *
		app->window->width * app->window->height)),
		"Failed to malloc framebuffer in resize");
	ft_memset(app->window->zbuffer, INT32_MAX, sizeof(uint32_t) *
		app->window->width * app->window->height);
	if (app->window->font != NULL)
		TTF_CloseFont(app->window->font);
	app->window->font = TTF_OpenFont(FONT, FONT_SIZE);
	error_check(app->window->font == NULL, TTF_GetError());
}

void			window_init(t_app *app)
{
	error_check((app->window = (t_window*)malloc(sizeof(t_window))) == NULL,
		"Window malloc failed");
	app->window->window = SDL_CreateWindow("Demo", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE);
	app->window->width = WIDTH;
	app->window->height = HEIGHT;
	error_check(app->window->window == NULL, SDL_GetError());
	app->window->renderer =
		SDL_CreateRenderer(app->window->window, -1, SDL_RENDERER_SOFTWARE);
	error_check(app->window->renderer == NULL, SDL_GetError());
	app->window->window_id = SDL_GetWindowID(app->window->window);
	app->window->is_hidden = false;
	app->window->frame = NULL;
	app->window->framebuffer = NULL;
	app->window->zbuffer = NULL;
	app->window->font = NULL;
	app->window->resized = false;
	recreate_frame(app);
	SDL_AddEventWatch(resize_callback, app);
}
