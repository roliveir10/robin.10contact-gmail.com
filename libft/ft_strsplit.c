/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roliveir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/10 16:26:28 by roliveir          #+#    #+#             */
/*   Updated: 2019/03/01 01:07:32 by roliveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static int	l(const char *s, int i, char c)
{
	int b;

	b = 0;
	while (s[i] != c && s[i] != '\t' && s[i])
	{
		b++;
		i++;
	}
	return (b);
}

static int	wo(const char *s, char c)
{
	int i;
	int count;

	i = 0;
	count = 0;
	while (s[i])
	{
		while ((s[i] == c || s[i] == '\t') && s[i])
			i++;
		if (s[i] != c && s[i] != '\t' && s[i])
		{
			while (s[i] != c && s[i] != '\t' && s[i])
				i++;
			count++;
		}
	}
	return (count);
}

char		**ft_strsplit(char const *s, char c)
{
	int		i;
	int		j;
	int		y;
	char	**t;

	if (!s || !(t = (char**)malloc(sizeof(char*) * (wo(s, c) + 1))))
		return (NULL);
	j = 0;
	i = 0;
	while (s[i])
	{
		while ((s[i] == c || s[i] == '\t') && s[i])
			i++;
		if (s[i])
		{
			y = 0;
			if (!(t[j] = (char*)malloc(sizeof(char) * l(s, i, c) + 1)))
				return (NULL);
			while (s[i] != c && s[i] != '\t' && s[i])
				t[j][y++] = s[i++];
			t[j++][y] = '\0';
		}
	}
	t[j] = 0;
	return (t);
}
