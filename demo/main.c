/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 17:13:23 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/17 18:44:02 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "demo.h"

static void			update_frame_buffer(t_app *app)
{
	ft_memset(app->window->framebuffer, 255,
		app->window->width * app->window->height * sizeof (uint32_t));
	//ToDo: Render whatever you wanna render to framebuffer
}

static void			draw_frame(t_app *app)
{

	if (app->window->resized)
	{
		recreate_frame(app);
		app->window->resized = false;
		while (app->window->is_hidden)
			SDL_PollEvent(NULL);
	}
	SDL_LockTexture(app->window->frame, NULL,
		(void**)&app->window->framebuffer,
		&app->window->pitch);
	update_frame_buffer(app);
	SDL_UnlockTexture(app->window->frame);
	SDL_RenderCopy(app->window->renderer, app->window->frame,
		NULL, NULL);
	SDL_RenderPresent(app->window->renderer);
}

static void			main_loop(t_app *app)
{
	SDL_Event	event;
	t_bool		is_running;
	uint32_t	time_since_start;
	uint32_t	fps;

	is_running = true;
	while (is_running)
	{
		time_since_start = SDL_GetTicks();
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN &&
				event.key.keysym.sym == SDLK_ESCAPE))
				is_running = false;
		}
		draw_frame(app);
		fps = capture_framerate(time_since_start);
		draw_fps(app, fps);
	}
}

static void			app_cleanup(t_app *app)
{
	SDL_DestroyRenderer(app->window->renderer);
	SDL_DestroyWindow(app->window->window);
	free(app->window);
	TTF_CloseFont(app->window->font);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

int					main(void)
{
	t_app	app;

	error_check(SDL_Init(SDL_INIT_VIDEO) != 0, SDL_GetError());
	error_check(TTF_Init() == -1, TTF_GetError());
	window_init(&app);
	main_loop(&app);
	app_cleanup(&app);
	return (0);
}
