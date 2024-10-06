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

void	ft_printf_unsigned(t_printf *restrict const data,
		const char *const digits, const uint8_t base, unsigned int long long nb)
{
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
	while (++i < 20)
	{
		if (data->buffer.i == FT_PRINTF_BUFFER_SIZE)
		{
			flush_buffer(data);
			if (data->return_val == -1)
				return ;
		}
		data->buffer.buffer[data->buffer.i++] = tmp[i];
	}
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

static double	f_handle_integer_part(double f, t_printf *restrict const data)
{
	double	tmp;
	int		div;

	while (f >= 1.0)
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
				return (f);
		}
		f -= (int)(tmp * div);
	}
	data->buffer.buffer[data->buffer.i++] = '.';
	return (f);
}

void	ft_printf_f(t_printf *restrict const data)
{
	double	f;

	f = va_arg(data->arg, double);
	if (f < 0)
	{
		data->buffer.buffer[data->buffer.i++] = '-';
		f *= -1;
	}
	if (f < 1.0)
		ft_printf_unsigned(data, "0123456789", 10, 0);
	f = f_handle_integer_part(f, data);
	if (data->return_val == -1)
		return ;
	f *= 100000;
	ft_printf_unsigned(data, "0123456789", 10, (unsigned)(f + 0.5));
}
