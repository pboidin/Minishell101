#include "minishell.h"

void	ft_perror_exit(const char *str)
{
	if (str == NULL)
		perror("minishell");
	else
		perror(str);
	exit(EXIT_FAILURE);
}

/* add built in to built in list */
static void	ft_add_blti(t_blti **head, char *name, int (*f)(char **))
{
	t_blti	*aux;
	t_blti	*new;

	new = (t_blti *)malloc(sizeof(t_blti));
	if (new == NULL)
		ft_perror_exit("malloc");
	new->name = name;
	new->f = f;
	new->next = NULL;
	if (*head != NULL)
	{
		aux = *head;
		while (aux->next)
			aux = aux->next;
		aux->next = new;
	}
	else
		*head = new;
}

/* initialise built ins */
static t_blti	*ft_init_blti(void)
{
	t_blti	*bi;

	bi = NULL;
	ft_add_blti(&bi, "cd", ft_ch_dir);
	ft_add_blti(&bi, "echo", ft_echo);
	ft_add_blti(&bi, "env", ft_env);
	ft_add_blti(&bi, "exit", ft_exit);
	ft_add_blti(&bi, "export", ft_export);
	ft_add_blti(&bi, "pwd", ft_working_dir);
	ft_add_blti(&bi, "unset", ft_unset);
	return (bi);
}