/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   throw_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/13 11:47:31 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/13 12:18:37 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parsing_error(char *str, char c)
{
	write(2, "Minishell: syntax error near unexpected token `", 47);
	write(2, str, ft_strlen(str));
	write(2,"'\n", 2);
}
