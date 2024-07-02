/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 14:30:35 by frapp             #+#    #+#             */
/*   Updated: 2024/04/18 10:34:54 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	char		*restrict	d;
	const char	*restrict	s;

	if (!dst)
		return (NULL);
	if (!src)
		return (dst);
	if (!dst && !src && n)
		return (dst);
	d = (char *restrict)dst;
	s = (const char *restrict)src;
	while (n--)
		*d++ = *s++;
	return (dst);
}

void	*ft_memdup(const void *src, size_t size)
{
	char	*new;

	new = malloc(size);
	ft_memcpy(new, src, size);
	return (new);
}
