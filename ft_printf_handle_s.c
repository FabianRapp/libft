/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_handle_s.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabian <fabian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 18:36:32 by frapp             #+#    #+#             */
/*   Updated: 2024/04/05 22:27:51 by fabian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_printf_handle_s(va_list *arg, int *written_bytes, int fd)
{
	char	*argument;

	argument = va_arg(*arg, char *);
	if (!argument)
	{
		if (write(fd, "(null)", 6) == -1)
			return (-1);
		(*written_bytes) += 6;
		return (0);
	}
	if (ft_strlen(argument))
	{
		if (write(fd, argument, ft_strlen(argument)) == -1)
			return (-1);
	}
	(*written_bytes) += ft_strlen(argument);
	return (0);
}
