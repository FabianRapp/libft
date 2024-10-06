#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# ifndef FT_PRINTF_BUFFER_SIZE
#  define FT_PRINTF_BUFFER_SIZE 128
# endif //FT_PRINTF_BUFFER_SIZE

# include <stddef.h>
# include <unistd.h>
# include <stdbool.h>
# include <stdarg.h>
# include <stdio.h>
# include <stdint.h>
# include <limits.h>

typedef struct s_printf_buffer
{
	char		buffer[FT_PRINTF_BUFFER_SIZE];
	int			i;
}	t_printf_buffer;

typedef struct s_printf
{
	t_printf_buffer	buffer;
	const char		*format;
	va_list			arg;
	int				return_val;
	int				fd;
}	t_printf;

int		ft_printf(const char *format_str, ...);

void	pattern(t_printf *restrict const data, const char type);
void	ft_printf_str(t_printf *restrict const data, const char *str);
void	ft_printf_unsigned(t_printf *restrict const data,
			const char *const digits,
			const uint8_t base, unsigned int long long nb);
void	ft_printf_di(t_printf *restrict const data, int nb);
void	ft_printf_f(t_printf *restrict const data);
#endif
