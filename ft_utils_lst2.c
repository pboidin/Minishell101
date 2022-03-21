#include "includes/minishell.h"

void	ft_lstdelone(t_env *lst, void (*del)(void*))
{
	if (!lst)
		return ;
	if (del)
		del(lst->value);
	free(lst);
}