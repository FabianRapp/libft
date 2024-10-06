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

static void	put_format_to_buf(t_printf *restrict const data)
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

static int	ft_printf_base(t_printf *restrict const data)
{
	while (*data->format && data->return_val != -1)
	{
		put_format_to_buf(data);
		if (!*data->format)
			break ;
		data->format++;
		pattern(data, *(data->format++));
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
