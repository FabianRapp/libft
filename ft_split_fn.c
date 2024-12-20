/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_fn.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabian <fabian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/08 01:18:40 by frapp             #+#    #+#             */
/*   Updated: 2024/04/05 22:27:51 by fabian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	get_str_count_fn(const char *s, bool is_sep(char))
{
	size_t	count;
	int		was_c;

	count = 0;
	was_c = 1;
	while (*s)
	{
		if (!was_c && is_sep(*s))
		{
			count++;
			was_c = 1;
		}
		else if (is_sep(*s))
			was_c = 1;
		else
			was_c = 0;
		s++;
	}
	if (!was_c)
		count++;
	return (count);
}

static int	append_next_sub_fn(char **sub_str, const char *s, bool is_sep(char))
{
	int		count;
	int		len;
	char	*found;

	found = ft_strchr_fn(s, is_sep);
	if (!found)
		len = ft_strlen(s);
	else
		len = found - s;
	if (len == 0)
		return (0);
	*sub_str = (char *)ft_calloc(len + 1, sizeof(char));
	if (!(*sub_str))
		return (-1);
	count = 0;
	while (s[count] && !is_sep(s[count]))
	{
		(*sub_str)[count] = s[count];
		count++;
	}
	return (count);
}

char	**ft_split_fn(char const *s, bool is_sep(char))
{
	size_t		str_count;
	char		**str_ar;
	int			i;
	int			s_increase;

	str_count = get_str_count_fn(s, is_sep);
	str_ar = (char **)ft_calloc(str_count + 1, sizeof(char *));
	if (!str_ar)
		return (NULL);
	i = 0;
	while (*s)
	{
		while (*s && is_sep(*s))
			s++;
		s_increase = append_next_sub_fn(str_ar + i, s, is_sep);
		if (s_increase == -1)
			return (free_2d_arr(str_ar), NULL);
		if (s_increase)
			i++;
		s += s_increase;
	}
	return (str_ar);
}
