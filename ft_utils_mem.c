#include "../includes/minishell.h"

void	*ft_memcpy(void *dst, const void *src, size_t memSize)
{
	unsigned char	*tmp1;
	unsigned char	*tmp2;

	tmp1 = (unsigned char *)src;
	tmp2 = (unsigned char *)dst;
	while (memSize > 0)
	{
		*tmp2++ = *tmp1++;
		memSize--;
	}
	return (dst);
}

void	*ft_realloc(void *ptr, size_t memSize)
{
	void	*new;

	if (!ptr)
		return (malloc(memSize));
	if (!memSize)
	{
		if (ptr)
		{
			free (ptr);
			ptr=NULL;
		}
		return (NULL);
	}
	new = malloc(memSize);
	if (!new)
		return (NULL);
	ft_memcpy(new, ptr, memSize);
	free (ptr);
	ptr = NULL;
	return (new);
}