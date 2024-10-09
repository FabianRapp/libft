/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_pattern1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 22:18:30 by frapp             #+#    #+#             */
/*   Updated: 2024/10/09 22:18:31 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static inline void	flush_buffer(t_printf *restrict const data)
{
	int	write_val;

	write_val = write(data->fd, data->buffer.buffer, data->buffer.i);
	if (write_val == -1)
		data->return_val = -1;
	data->return_val += data->buffer.i;
	data->buffer.i = 0;
}

void	ft_printf_str(t_printf *restrict const data, const char *str)
{
	int				buffer_i;
	char			*buffer;

	buffer = data->buffer.buffer;
	buffer_i = data->buffer.i;
	while (*str)
	{
		buffer[buffer_i++] = *(str++);
		if (buffer_i == FT_PRINTF_BUFFER_SIZE)
		{
			data->buffer.i = buffer_i;
			buffer_i = 0;
			flush_buffer(data);
			if (data->return_val == -1)
				return ;
		}
	}
	data->buffer.i = buffer_i;
}

static void	pattern2(t_printf *restrict const data, const char type)
{
	unsigned long long int	nb;

	if (type == 'd' || type == 'i')
		ft_printf_di(data, va_arg(data->arg, int));
	else if (type == 'f')
		ft_printf_f(data);
	else if (type == 'u' || type == 'X' || type == 'x')
	{
		nb = va_arg(data->arg, unsigned);
		if (type == 'u')
			ft_printf_unsigned(data, "0123456789", 10, nb);
		else if (type == 'X')
			ft_printf_unsigned(data, "0123456789ABCDEF", 16, nb);
		else
			ft_printf_unsigned(data, "0123456789abcdef", 16, nb);
	}
	else if (type == 'c')
		data->buffer.buffer[data->buffer.i++] = va_arg(data->arg, int);
	else if (type == '%')
		data->buffer.buffer[data->buffer.i++] = '%';
	else
		data->return_val = -1;
}

//assumes data->format[0] to be the format specifier after a '%'
//assumes atleast 1 space in the output buffer
void	pattern(t_printf *restrict const data, const char type)
{
	const char				*str;
	unsigned long long int	nb;

	if (type == 's')
	{
		str = va_arg(data->arg, char *);
		if (!str)
			ft_printf_str(data, "(null)");
		else
			ft_printf_str(data, str);
	}
	else if (type == 'p')
	{
		nb = va_arg(data->arg, unsigned long long);
		if (!nb)
			ft_printf_str(data, "(nil)");
		else
		{
			ft_printf_str(data, "0x");
			ft_printf_unsigned(data, "0123456789abcdef", 16, nb);
		}
	}
	else
		pattern2(data, type);
}
