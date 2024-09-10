#include "ft_printf.h"
#include <stdio.h>
int	main(void)
{
	char *s2 = "";
	int expect = printf("%f\n", -1.2345);
	int mine = ft_printf("%f\n", -1.2345);
	if (expect != mine)
		printf("mine: %d; expected: %d\n", mine, expect);
	return (0);
}
