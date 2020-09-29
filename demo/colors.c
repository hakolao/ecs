/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 18:07:58 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/30 01:32:01 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "demo_common.h"

uint32_t		rgba_to_u32(SDL_Color color)
{
	return (((color.r & 255) << 24) | ((color.g & 255) << 16) |
		((color.b & 255) << 8) | (color.a & 255));
}

SDL_Color		u32_to_rgba(uint32_t color)
{
	SDL_Color	sdl_color;

	sdl_color.r = (color >> 24) & 255;
	sdl_color.g = (color >> 16) & 255;
	sdl_color.b = (color >> 8) & 255;
	sdl_color.a = (color & 255);
	return (sdl_color);
}

uint32_t		color_blend_u32(uint32_t color1, uint32_t color2, float ratio)
{
	float		i_ratio;
	SDL_Color	sdl_color1;
	SDL_Color	sdl_color2;
	SDL_Color	new_color;

	if (ratio > 1.f)
		ratio = 1.f;
	else if (ratio < 0.f)
		ratio = 0.f;
	i_ratio = 1.f - ratio;
	sdl_color1 = u32_to_rgba(color1);
	sdl_color2 = u32_to_rgba(color2);
	if (sdl_color2.a == 0)
		return (color1);
	new_color.r = (int)((sdl_color1.r * i_ratio) + (sdl_color2.r * ratio));
	new_color.g = (int)((sdl_color1.g * i_ratio) + (sdl_color2.g * ratio));
	new_color.b = (int)((sdl_color1.b * i_ratio) + (sdl_color2.b * ratio));
	new_color.a = (int)((sdl_color1.a * i_ratio) + (sdl_color2.a * ratio));
	return (rgba_to_u32(new_color));
}
