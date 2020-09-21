/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 17:13:23 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/21 16:03:09 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "demo.h"

/*
** Set 4 colors at a time for speed
*/

static void			clear_frame(t_app *app)
{
	int			i;
	uint32_t	color;

	i = 0;
	color = 0xFFFFFFFF;
	while (i < app->window->width * app->window->height)
	{
		app->window->framebuffer[i] = color;
		app->window->framebuffer[i + 1] = color;
		app->window->framebuffer[i + 2] = color;
		app->window->framebuffer[i + 3] = color;
		i += 4;
	}
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
	SDL_UpdateTexture(app->window->frame, NULL, app->window->framebuffer,
		app->window->width * 4);
	SDL_RenderCopy(app->window->renderer, app->window->frame,
		NULL, NULL);
	SDL_RenderPresent(app->window->renderer);
}

static void			main_loop(t_app *app)
{
	SDL_Event	event;
	t_bool		is_running;

	is_running = true;
	app->world = ecs_world_create("Demo world", MAX_ENTITIES);
	ft_printf("Created world: %s\n", app->world->name);
	systems_create(app);
	entities_create_up_to_max(app);
	ft_printf("Created %d entities\n", app->world->num_entities);
	while (is_running)
	{
		app->info.time_since_start = SDL_GetTicks();
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN &&
				event.key.keysym.sym == SDLK_ESCAPE))
				is_running = false;
		}
		clear_frame(app);
		systems_params_update(app);
		ecs_systems_run_parallel(NUM_THREADS, app->world,
			system_forces | system_render | system_reset);
		draw_fps(app);
		draw_frame(app);
		app->info.delta_time = SDL_GetTicks() - app->info.time_since_start;
		app->info.fps = capture_framerate(app->info.delta_time);
	}
}

static void			app_cleanup(t_app *app)
{
	free(app->window->framebuffer);
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

	app.info.fps = 0;
	app.info.delta_time = 0;
	error_check(SDL_Init(SDL_INIT_VIDEO) != 0, SDL_GetError());
	error_check(TTF_Init() == -1, TTF_GetError());
	window_init(&app);
	main_loop(&app);
	app_cleanup(&app);
	return (0);
}
