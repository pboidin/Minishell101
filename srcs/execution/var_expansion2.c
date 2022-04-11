#include "minishell.h"

char	*find_var(char *var, t_info *info)
{
	char	*value;
	t_env	*current_env;
	t_var	*current_var;

	current_env = info->env;
	while (current_env)
	{
		if (!ft_strcmp(&var[1], current_env->name))
		{
			value = ft_strdup(current_env->value);
			return (value);
		}
		current_env = current_env->next;
	}
	current_var = info->local_var;
	while (current_var)
	{
		if (!ft_strcmp(&var[1], current_var->name))
		{
			value = ft_strdup(current_var->value);
			return (value);
		}
		current_var = current_var->next;
	}
	return ((char *)ft_calloc(1, sizeof(char)));
}
/*
t_block	**create_t_tab(char *str)
{
	size_t	i;
	t_block	**words_tab;

	words_tab = (t_block **)ft_calloc(2, sizeof(t_block *));
	if (!words_tab)
		return (NULL);
	word_count = count_words_var_expansion(str);
	words_tab[0] = (t_block *)ft_calloc((word_count + 1), sizeof(t_block));
	if (!words_tab[0])
		return (free_t_block_tab(words_tab), NULL);
	i = 0;
	index = 0;
	while (i < word_count)
	{
		words_tab[0][i].str = add_redirect_word(str, &index);
		if (!words_tab[0][i].str)
			return (free_t_block_tab(words_tab), NULL);
		i++;
	}
	return (words_tab);
}

t_block	**add_args_word(char *str, t_info *info, int expand)
{
	size_t	word_count;
	t_block	**words_tab;
	size_t	i;
	size_t	j;
	size_t	index;
	char	**var;

	words_tab = create_t_tab(str);
	if (!words_tab)
		return (perror("Malloc error"), NULL);
	j = 0;
	i = 0;
	while (words_tab[j][i].str)
	{
		if (words_tab[j][i].str[0] == '$' && expand)
		{
			words_tab[j][i].var = 1;
			var = replace_redirect_var(words_tab[j], i, info);
			if (!var || !var[0])
			{
				free_t_block_tab(words_tab);
				if (var)
					free(var);
				return (NULL);
			}
			if (char_tab_size(var) == 1)
			{
				free(words_tab[j][i].str);
				words_tab[j][i].str = var[0];
				free(var);
			}
			else
			{
				word_count = split_tab_var(&words_tab, j, i, var);
				if (!word_count)
					return (free_t_block_tab(words_tab), NULL);
				j += (word_count - 1);
				i = 1;
			}
		}
		else if (words_tab[j][i].str[0] == 39)
		{
			words_tab[j][i].spl_qu = 1;
			if (remove_qu(words_tab[j], i))
				return (free_t_block_tab(words_tab), NULL);
		}
		else if (words_tab[j][i].str[0] == '"')
		{
			words_tab[j][i].dbl_qu = 1;
			if (expand && expand_dbl_qu_var(words_tab[j], i, info))
				return (free_t_block_tab(words_tab), NULL);
			if (remove_qu(words_tab[j], i))
				return (free_t_block_tab(words_tab), NULL);
		}
		i++;
	}
	return (words_tab);
}*/
