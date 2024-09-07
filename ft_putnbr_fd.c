/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabian <fabian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/08 20:22:53 by frapp             #+#    #+#             */
/*   Updated: 2024/04/05 22:27:51 by fabian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	fill_div(int *dividend)
{
	dividend[0] = 1;
	dividend[1] = 10;
	dividend[2] = 100;
	dividend[3] = 1000;
	dividend[4] = 10000;
	dividend[5] = 100000;
	dividend[6] = 1000000;
	dividend[7] = 10000000;
	dividend[8] = 100000000;
	dividend[9] = 1000000000;
}

static int	start(int n, int fd, int *written_chars)
{
	int		pow;

	*written_chars = 0;
	if (n < 0)
		*written_chars = write(fd, "-", 1);
	else if (n == 0)
		*written_chars = write(fd, "0", 1);
	if (*written_chars < 0)
		return (-1);
	pow = 0;
	while (n)
	{
		n /= 10;
		pow++;
	}
	return (pow);
}

int	ft_putnbr_fd(int n, int fd)
{
	int		pow;
	int		dividend[10];
	char	digit[1];
	int		temp;
	int		written_chars;

	fill_div(dividend);
	pow = start(n, fd, &written_chars);
	while (written_chars != -1 && pow)
	{
		if (n > 0)
			temp = (n / dividend[pow - 1]);
		else
			temp = -1 * (n / dividend[pow - 1]);
		*digit = '0' + temp;
		n -= (temp * dividend[pow-- - 1]);
		if (write(fd, digit, 1) == -1)
			return (-1);
		written_chars++;
	}
	return (written_chars);
}
