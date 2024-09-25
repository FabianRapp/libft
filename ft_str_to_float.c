#include "libft.h"

static size_t	nb_len(char *str_nb)
{
	size_t	i;

	i = 0;
	while (str_nb[i] && ft_isdigit(str_nb[i]))
		i++;
	return (i);
}


float	str_to_float(char *str)
{
	int		integer;
	float	fraction;
	int		sign;

	sign = 1;
	if (*str == '-')
	{
		sign = -1;
		str++;
	}
	integer = ft_atoi(str);
	while (ft_isdigit(*str))
		str++;
	if (*str == '.')
		str++;
	else
		return ((double)integer * sign);
	if (!ft_isdigit(*str))
		return ((double)integer * sign);
	fraction = (double)ft_atoi(str);
	fraction /= ft_powint(10, nb_len(str));
	return ((fraction + integer) * sign);
}
