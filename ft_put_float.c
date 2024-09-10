#include "libft.h"
#include <limits.h>

int	ft_put_float(int fd, float f, size_t precision)
{
	double	tmp;
	int		div;
	int		count;
	int		write_ret;

	write_ret = 0;
	if (f < 0)
		write_ret = write(fd, "-", 1);
	if (f < 0)
		f *= -1;
	count = write_ret;
	while (f > 1.0 && write_ret != -1)
	{
		tmp = f;
		div = 1;
		while (tmp > INT_MAX)
		{
			tmp = f / div;
			div *= 10;
		}
		write_ret = ft_putnbr_fd((int)tmp, fd);
		count += write_ret;
		f -= (int)(tmp * div);
	}
	if (write_ret == -1)
		return (-1);
	if (!precision)
		return (count);
	write_ret = write(fd, ".", 1);
	if (write_ret == -1)
		return (-1);
	f *= ft_powint(10, precision);
	write_ret = ft_putnbr_fd((int)(f + 0.5), fd);
	if (write_ret == -1)
		return (-1);
	return (count + 1 + write_ret);
}

