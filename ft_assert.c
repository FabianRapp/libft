
#include "libft.h"

//how to call to crash the programm if a is not 10:
//ft_assert(a == 10, __FILE__, __LINE__, "a is not 10);
#if defined(__x86_64__) || defined(__i386__)

void	ft_assert(bool cond, char *file, int line, char *msg)
{
	if (cond)
		return ;
	ft_fprintf(2, "Assert in %s line %d: %s\n:", file, line, msg);
	__asm__("INT3\n\t");
}
#elif defined(__arm__)
void	ft_assert(bool cond, char *file, int line, char *msg)
{
	if (cond)
		return ;
	ft_fprintf(2, "Assert in %s line %d: %s\n:", file, line, msg);
	__asm__("BKPT #0");
}

#else
void	ft_assert(bool cond, char *file, int line, char *msg)
{
	if (cond)
		return ;
	ft_fprintf(2, "Assert in %s line %d: %s\n:", file, line, msg);
	exit(1);
}

#endif //ARCHITECTURE
