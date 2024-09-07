#include "libft.h"

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
	if (ft_strchr(str, '\n'))
		fraction /= ft_powint(10, ft_strlen(str) - 1);
	else
		fraction /= ft_powint(10, ft_strlen(str));
	return ((fraction + integer) * sign);
}


