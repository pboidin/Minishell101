/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inline_expansion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/13 10:50:59 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/13 10:51:01 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_one_char_var(char c, char **dst, size_t *len, t_info *info)
{
	if (c == '?')
		*dst = ft_itoa(info->status);
	else
		*dst = (char *)ft_calloc(1, sizeof(char));
	*len += 1;
}

static void	get_var_inline(char *str, char **dst, size_t *len, t_info *info)
{
	char	holder;
	size_t	i;

	i = 1;
	while (str[i + *len] && (ft_isalnum(str[i + *len]) || str[i + *len] == '_'))
		*len += 1;
	holder = str[i + *len];
	str[i + *len] = '\0';
	*dst = find_var(str, info);
	str[i + *len] = holder;
}

static int	insert_var_inline(t_block *tab, int *j, char *var, size_t len)
{
	char	*tmp;

	tab[0].str[*j] = '\0';
	tmp = ft_strjoin(tab[0].str, var);
	free(var);
	if (!tmp)
		return (1);
	var = ft_strjoin(tmp, &tab[0].str[*j + len + 1]);
	*j = ft_strlen(var) - 1;
	free(tmp);
	if (!var)
		return (1);
	free(tab[0].str);
	tab[0].str = var;
	return (0);
}

int	inline_expansion(t_block *tab, size_t i, t_info *info)
{
	size_t	len;
	char	*tmp;
	int		j;

	j = 0;
	while (tab[i].str[j])
	{
		if (tab[i].str[j] == '$' && (ft_isalnum(tab[i].str[j + 1])
				|| tab[i].str[j + 1] == '?' || tab[i].str[j + 1] == '_'))
		{
			tmp = NULL;
			len = 0;
			if (tab[i].str[j + 1] == '?' || ft_isdigit(tab[i].str[j + 1]))
				handle_one_char_var(tab[i].str[j + 1], &tmp, &len, info);
			else
				get_var_inline(&tab[i].str[j], &tmp, &len, info);
			if (!tmp)
				return (perror("Malloc error"), 1);
			if (insert_var_inline(&tab[i], &j, tmp, len))
				return (perror("Malloc error"), 1);
		}
		j++;
	}
	return (0);
}
