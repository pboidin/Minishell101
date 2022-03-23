/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils_lst2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piboidin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/22 17:29:13 by piboidin          #+#    #+#             */
/*   Updated: 2022/03/22 17:29:15 by piboidin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_lstdelone(t_env *lst, void (*del)(void*))
{
	if (!lst)
		return ;
	if (del)
		del(lst->value);
	free(lst);
}
