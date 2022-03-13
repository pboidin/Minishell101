#include "includes/minishell.h"

void	*ft_memset(void *target, int char_to_set, size_t n)
{
	char	*str;
	size_t	i;

	i = 0;
	str = target;
	while (i < n)
	{
		str[i] = (unsigned char)char_to_set;
		i++;
	}
	return (target);
}
