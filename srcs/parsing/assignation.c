/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assignation.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/13 20:29:03 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/13 21:28:48 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_assignation(char	*str)
{
	size_t	j;
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			break ;
		i++;
	}
	if (str[i] != '=')
		return (0);
	if (!(ft_isalpha(str[0]) || str[0] == '_'))
		return (0);
	j = 1;
	while (j < i)
	{
		if (!(ft_isalnum(str[i]) || str[i] == '_'))
			return (0);
		j++;
	}
	return (1);
}

void	throw_assignation_error(char *str)
{
	int			i;
	t_tokens	toks;

	init_tokens(&toks);
	i = -1;
	while (str[++i])
	{
		save_token(str[i], &toks);
		if (!toks.dbl_qu && !toks.spl_qu && (str[i] == '(' || str[i] == ')'
				|| str[i] == '|' || (str[i] == '&' && str[i + 1] == '&')
				|| str[i] == '<' || str[i] == '>'))
			break ;
	}
	if ((str[i] == '|' && str[i + 1] == '|')
		|| (str[i] == '&' && str[i + 1] == '&')
		|| (str[i] == '<' && str[i + 1] == '<')
		|| (str[i] == '>' && str[i + 1] == '>'))
		str[i + 2] = '\0';
	else
		str[i + 1] = '\0';
	parsing_error(-1, &str[i], NULL);
}

int	is_valid_assignation(char *str)
{
	int	i;
//	int	j;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	i++;
//	j = i;
/*	if (str[j] == '(')
	{
		skip_closing_parenth(str, &j);
		if (!is_valid_arg(&str[i]))
			return (throw_assignation_error(&str[i]), 0);
		str[i] = '(';
		str[j] = ')';
		j++;
	}*/
	if (str[i] && !is_valid_arg(&str[i]))
		return (throw_assignation_error(&str[i]), 0);
	return (1);
}
