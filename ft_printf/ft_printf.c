# include "ft_printf.h"

#include <stdio.h>

void	flush_buffer(t_printf *restrict const data)
{
	int	write_val;

	write_val = write(data->fd, data->buffer.buffer, data->buffer.i);
	if (write_val == -1)
		data->return_val = -1;
	data->return_val += data->buffer.i;
	data->buffer.i = 0;
}

void	put_format_to_buf(t_printf *restrict const data)
{
	char		*buffer;
	int			buffer_i;
	const char	*format;

	buffer_i = data->buffer.i;
	buffer = data->buffer.buffer;
	format = data->format;
	while (*format && *format != '%')
	{
		buffer[buffer_i++] = *(format++);
		if (buffer_i == FT_PRINTF_BUFFER_SIZE)
		{
			data->buffer.i = buffer_i;
			buffer_i = 0;
			flush_buffer(data);
			if (data->return_val == -1)
				return ;
		}
	}
	data->format = format;
	data->buffer.i = buffer_i;
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

void	ft_printf_unsigned(t_printf *restrict const data,
		const char *const digits, const uint8_t base, unsigned int long long nb)
{
	int				buffer_i;
	char			*buffer;
	char			tmp[20];
	int				i;

	if (base != 10 && base != 16)
		__builtin_unreachable();
	i = 19;
	while (nb || i == 19)
	{
		tmp[i--] = digits[nb % base];
		nb /= base;
	}
	buffer_i = data->buffer.i;
	buffer = data->buffer.buffer;
	while (++i < 20)
	{
		if (buffer_i == FT_PRINTF_BUFFER_SIZE)
		{
			data->buffer.i = buffer_i;
			buffer_i = 0;
			flush_buffer(data);
			if (data->return_val == -1)
				return ;
		}
		buffer[buffer_i++] = tmp[i];
	}
	data->buffer.i = buffer_i;
}

void	ft_printf_di(t_printf *restrict const data, int nb)
{
	if (nb == INT_MIN)
		return (ft_printf_str(data, "-2147483648"));
	if (nb < 0)
	{
		data->buffer.buffer[data->buffer.i++] = '-';
		nb *= -1;
	}
	ft_printf_unsigned(data, "0123456789", 10, (unsigned)nb);
}

void	ft_printf_f(t_printf *restrict const data)
{
	double	tmp;
	double	f;
	int		div;

	f = va_arg(data->arg, double);
	if (f < 0)
	{
		data->buffer.buffer[data->buffer.i++] = '-';
		f *= -1;
	}
	while (f > 1.0)
	{
		tmp = f;
		div = 1;
		while (tmp > INT_MAX)
		{
			tmp = f / div;
			div *= 10;
		}
		ft_printf_unsigned(data, "0123456789", 10, (unsigned)tmp);
		if (data->buffer.i == FT_PRINTF_BUFFER_SIZE)
		{
			flush_buffer(data);
			if (data->return_val == -1)
				return ;
		}
		f -= (int)(tmp * div);
	}
	data->buffer.buffer[data->buffer.i++] = '.';
	f *= 1000000;
	ft_printf_unsigned(data, "0123456789", 10, (unsigned)(f + 0.5));
}

//assumes data->format[0] to be the format specifier after a '%'
//assumes atleast 1 space in the output buffer
void	pattern(t_printf *restrict const data)
{
	char					type;
	const char				*str;
	unsigned long long int	nb;

	type = *(data->format++);
	if (type == 's')
	{
		str = va_arg(data->arg, char *);
		if (!str)
			ft_printf_str(data, "(null)");
		else
			ft_printf_str(data, str);
	}
	else if (type == 'd' || type == 'i')
		ft_printf_di(data, va_arg(data->arg, int));
	else if (type == 'f')
		ft_printf_f(data);
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
	else if (type== '%')
		data->buffer.buffer[data->buffer.i++] = '%';
	else
		data->return_val = -1;
}

int	ft_printf_base(t_printf *restrict const data)
{

	while (*data->format && data->return_val != -1)
	{
		put_format_to_buf(data);
		if (!*data->format)
			break ;
		pattern(data);
		if (data->buffer.i == FT_PRINTF_BUFFER_SIZE)
		{
			flush_buffer(data);
			data->buffer.i = 0;
			if (data->return_val == -1)
				return (va_end(data->arg), -1);
		}
	}
	flush_buffer(data);
	return (va_end(data->arg), data->return_val);
}

int	ft_fprintf(int fd, const char *format_str, ...)
{
	t_printf	data;

	data.return_val = 0;
	data.format = format_str;
	data.buffer.i = 0;
	data.fd = fd;
	va_start(data.arg, format_str);
	return (ft_printf_base(&data));
}

int	ft_printf(const char *format_str, ...)
{
	t_printf	data;

	data.return_val = 0;
	data.format = format_str;
	data.buffer.i = 0;
	data.fd = 1;
	va_start(data.arg, format_str);
	return (ft_printf_base(&data));
}
