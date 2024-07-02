/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabian <fabian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/08 00:36:47 by frapp             #+#    #+#             */
/*   Updated: 2024/04/05 22:27:51 by fabian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t		len2;
	char		*sub_str;

	len2 = ft_strlen(s);
	if (len2 <= start)
		len = 0;
	else
	{
		s += start;
		len2 -= start;
	}
	if (len2 < len)
		len = len2;
	sub_str = (char *)ft_calloc((len + 1), sizeof(char));
	if (!sub_str)
		return (NULL);
	ft_strlcpy(sub_str, s, len + 1);
	return (sub_str);
}
