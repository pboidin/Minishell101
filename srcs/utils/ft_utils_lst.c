#include "../../includes/minishell.h"

void	ft_lstclear(t_env **lst, void (*del)(void *))
{
	void	*prev;

	if (!lst || !del)
		return ;
	while (*lst)
	{
		del((*lst)->value);
		prev = *lst;
		*lst = (*lst)->next;
		free(prev);
	}
	*lst = NULL;
}

t_env	*ft_lstlast(t_env *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	ft_lstadd_back(t_env **lst, t_env *new)
{
	if (!lst || !new)
		return ;
	if (*lst)
		ft_lstlast(*lst)->next = new;
	else
		*lst = new;
}

t_env	*ft_lstnew(void *data)
{
	t_env	*newlst;

	newlst = (t_env *)malloc(sizeof(t_env));
	if (!newlst)
		return (NULL);
	newlst->value = data;
	newlst->next = NULL;
	return (newlst);
}

int	ft_lstsize(t_env *lst)
{
	size_t	len;

	if (!lst)
		return (0);
	len = 0;
	while (lst && ++len)
		lst = lst->next;
	return (len);
}
