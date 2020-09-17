/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rand.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 19:36:38 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/17 19:50:49 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** https://stackoverflow.com/questions/3062746/
** special-simple-random-number-generator
*/

int			ft_rand(int seed)
{
	seed = (1103515245 * seed + 12345) % (1 << 31);
	return (seed);
}
