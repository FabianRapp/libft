#include "libft.h"

int	ft_put_float(int fd, float nb, size_t precision)
{
	int	count;

	count = ft_putnbr_fd((int)nb, fd);
	if (count < 0)
		return (-1);
	if (precision && write(fd, ".", 1) == -1)
		return (-1);
	if (precision)
		count += precision + 1;
	if (nb < 0)
		nb *= -1;
	nb -= (int)nb;
	while (precision)
	{
		nb *= 10;
		if (ft_putnbr_fd((int)nb, fd) == -1)
			return (-1);
		precision--;
	}
	return (count);
}
