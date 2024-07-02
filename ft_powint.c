/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_powint.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabian <fabian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 04:16:33 by frapp             #+#    #+#             */
/*   Updated: 2024/04/05 22:27:51 by fabian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_powint(int base, int expo)
{
	int	result;
	int	i;

	i = 0;
	result = 1;
	if (expo >= 0)
	{
		while (i < expo)
		{
			result *= base;
			i++;
		}
	}
	else
	{
		while (i < -expo)
		{
			result *= base;
			i++;
		}
		result = 1 / result;
	}
	return (result);
}
