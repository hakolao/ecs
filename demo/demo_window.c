/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   demo_window.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 18:00:29 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/17 18:46:20 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "demo.h"

static int		resize_callback(void *data, SDL_Event *event)
{
	t_window	*window;
	t_app 		*app;

	if (event->type == SDL_WINDOWEVENT &&
		(event->window.event == SDL_WINDOWEVENT_RESIZED ||
		 event->window.event == SDL_WINDOWEVENT_MINIMIZED ||
		 event->window.event == SDL_WINDOWEVENT_MAXIMIZED ||
		 event->window.event == SDL_WINDOWEVENT_SHOWN ||
		 event->window.event == SDL_WINDOWEVENT_HIDDEN))
		{
		window = (t_window*)data;
		if (event->window.windowID == window->window_id)
		{
			app = (t_app*)(window->parent);
			app->window->resized = true;
			SDL_GetWindowSize(window->window, &app->window->width,
				&app->window->height);
			if (event->window.event == SDL_WINDOWEVENT_HIDDEN)
				window->is_hidden = true;
			else if (event->window.event == SDL_WINDOWEVENT_SHOWN)
				window->is_hidden = false;
		}
	}
	return 0;
}

void			recreate_frame(t_app *app)
{
	if (app->window->frame != NULL)
		SDL_DestroyTexture(app->window->frame);
	app->window->frame = SDL_CreateTexture(app->window->renderer,
		PIXEL_FORMAT, SDL_TEXTUREACCESS_STREAMING, app->window->width,
		app->window->height);
	error_check(app->window->frame == NULL, SDL_GetError());
	if (app->window->font != NULL)
		TTF_CloseFont(app->window->font);
	app->window->font = TTF_OpenFont(FONT,
		get_relative_font_size(app, FONT_SIZE));
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
	app->window->parent = app;
	app->window->is_hidden = false;
	app->window->frame = NULL;
	app->window->framebuffer = NULL;
	app->window->font = NULL;
	app->window->resized = false;
	recreate_frame(app);
	SDL_AddEventWatch(resize_callback, app->window);
}
