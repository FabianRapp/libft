
#include "libft.h"

//how to call to crash the programm if a is not 10:
//ft_assert(a == 10, __FILE__, __LINE__, "a is not 10);
void	ft_assert(bool cond, char *file, int line, char *msg)
{
	if (cond)
		return ;
	ft_fprintf(2, "Assert in %s line %d: %s\n:", file, line, msg);
	__asm__("INT3\n\t");
	// exit(1);
}
