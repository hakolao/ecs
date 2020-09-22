/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 17:13:23 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/22 12:47:43 by ohakola          ###   ########.fr       */
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
		app->window->zbuffer[i] = INT32_MAX;
		app->window->zbuffer[i + 1] = INT32_MAX;
		app->window->zbuffer[i + 2] = INT32_MAX;
		app->window->zbuffer[i + 3] = INT32_MAX;
		i += 4;
	}
}

static void			draw_frame(t_app *app)
{
	SDL_UpdateTexture(app->window->frame, NULL, app->window->framebuffer,
		app->window->width * 4);
	SDL_RenderCopy(app->window->renderer, app->window->frame,
		NULL, NULL);
	SDL_RenderPresent(app->window->renderer);
}

static void			recreate_after_resize(t_app *app)
{
	ecs_world_destroy(app->world);
	app->world = ecs_world_create(NAME, MAX_ENTITIES);
	recreate_frame(app);
	systems_create(app);
	entities_create_up_to_max(app);
	app->window->resized = false;
	while (app->window->is_hidden)
		SDL_PollEvent(NULL);
}

/*
** Zbuffer check must happen before system_render, to prevent artifacts.
*/

static void			systems_run(t_app *app)
{
	systems_params_update(app);
	ecs_systems_run_parallel(NUM_THREADS, app->world, system_zbuffer);
	ecs_systems_run_parallel(NUM_THREADS, app->world,
		system_forces | system_render | system_reset);
}

static void			main_loop(t_app *app)
{
	SDL_Event	event;
	t_bool		is_running;

	is_running = true;
	app->world = ecs_world_create(NAME, MAX_ENTITIES);
	ft_printf("Created world: %s\n", app->world->name);
	systems_create(app);
	entities_create_up_to_max(app);
	ft_printf("Created %d entities\n", app->world->num_entities);
	while (is_running)
	{
		// SDL_GetPerformanceCounter();
		app->info.time_since_start = SDL_GetTicks();
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN &&
				event.key.keysym.sym == SDLK_ESCAPE))
				is_running = false;
			if ((event.type == SDL_KEYDOWN && event.key.repeat == false))
				if (event.key.keysym.sym == SDLK_p)
					app->is_gravity = !app->is_gravity;
			// !Note Must be here so resize doesn't cause a segfault due to some
			// !component data being dependent on window dimensions
			// !(intentionally)
			if (app->window->resized)
				recreate_after_resize(app);
		}
		clear_frame(app);
		systems_run(app);
		draw_fps(app);
		draw_frame(app);
		app->info.delta_time = SDL_GetTicks() - app->info.time_since_start;
		app->info.fps = capture_framerate(app->info.delta_time);
	}
}

static void			app_cleanup(t_app *app)
{
	ecs_world_destroy(app->world);
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
	app.is_gravity = true;
	error_check(SDL_Init(SDL_INIT_VIDEO) != 0, SDL_GetError());
	error_check(TTF_Init() == -1, TTF_GetError());
	window_init(&app);
	main_loop(&app);
	app_cleanup(&app);
	return (0);
}
