/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/08 00:47:03 by frapp             #+#    #+#             */
/*   Updated: 2024/04/18 12:45:12 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	size1;
	size_t	size2;
	char	*new_str;

	size1 = ft_strlen(s1);
	size2 = ft_strlen(s2);
	new_str = (char *) ft_calloc((size1 + size2 + 1), sizeof(char));
	if (!new_str)
		return (NULL);
	ft_strlcpy(new_str, s1, size1 + 1);
	ft_strlcpy(new_str + size1, s2, size2 + 1);
	return (new_str);
}

void	*ft_memjoin(void *src1, void *src2, int size1, int size2)
{
	uint8_t	*return_val;

	return_val = malloc((size1 + size2) * 2);
	if (!return_val)
		return (NULL);
	ft_memcpy(return_val, src1, size1);
	ft_memcpy(return_val + size1, src2, size2);
	return (return_val);
}

// returns false on malloc fail
bool	ft_strjoin_inplace(char **s1, char const *s2)
{
	size_t	size1;
	size_t	size2;
	char	*new_str;

	if (!s2 || !*s2)
		return (true);
	size1 = 0;
	size2 = 0;
	if (*s1)
		size1 = ft_strlen(*s1);
	if (s2)
		size2 = ft_strlen(s2);
	new_str = (char *) ft_calloc((size1 + size2 + 1), sizeof(char));
	if (!new_str)
	{
		free(*s1);
		*s1 = NULL;
		return (false);
	}
	ft_strlcpy(new_str, *s1, size1 + 1);
	ft_strlcpy(new_str + size1, s2, size2 + 1);
	free(*s1);
	*s1 = new_str;
	return (true);
}

// returns false on malloc fail
bool	ft_strjoin_inplace_char(char **s1, char const s2)
{
	size_t	size1;
	size_t	size2;
	char	*new_str;

	if (!s1 || !s2)
		return (true);
	size1 = 0;
	size2 = 1;
	if (*s1)
		size1 = ft_strlen(*s1);
	new_str = (char *) ft_calloc((size1 + size2 + 1), sizeof(char));
	if (!new_str)
	{
		free(*s1);
		*s1 = NULL;
		return (false);
	}
	if (*s1)
		ft_strlcpy(new_str, *s1, size1 + 1);
	new_str[size1] = s2;
	if (s2)
		new_str[size1 + 1] = 0;
	free(*s1);
	*s1 = new_str;
	return (true);
}
