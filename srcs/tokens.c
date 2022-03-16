/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 14:57:09 by bdetune           #+#    #+#             */
/*   Updated: 2022/03/16 14:58:07 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_tokens(t_tokens *tokens)
{
	tokens->par = 0;
	tokens->spl_qu = 0;
	tokens->dbl_qu = 0;
}

int	save_token(char c, t_tokens *toks)
{
	if (c == '"' && !toks->spl_qu)
		toks->dbl_qu ^= 1;
	else if (c == 39 && !toks->dbl_qu)
		toks->spl_qu ^= 1;
	else if (c == '(' && !toks->spl_qu && !toks->dbl_qu)
		tok->par += 1;
	else if (c == ')' && !toks->spl_qu && !toks->dbl_qu)
	{
		if (!toks->par)
			return (2);
		toks->par -= 1;
	}
	else
		return (0);
	return (1);
}
