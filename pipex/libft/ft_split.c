/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suroh <suroh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 10:41:26 by suroh             #+#    #+#             */
/*   Updated: 2024/12/03 00:42:19 by suroh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_blockcounter(const char *s, char c)
{
	int	i;
	int	counter;

	i = 0;
	counter = 0;
	if (!s)
		return (0);
	while (s[i] != '\0')
	{
		while (s[i] == c)
			i++;
		if (s[i])
			counter++;
		while (s[i] && s[i] != c)
			i++;
	}
	return (counter);
}

static char	**ft_free(char **str, int size)
{
	int	i;

	i = 0;
	while (i < size)
		free(str[i++]);
	free(str);
	return (NULL);
}

static const char	*ft_skipper(const char *s, char c)
{
	while (*s == c)
		s++;
	return (s);
}

static char	*allocate_block(const char *s, char c)
{
	int		count;
	char	*block;

	count = 0;
	while (s[count] && s[count] != c)
		count++;
	block = (char *)ft_calloc(sizeof(char), (count + 1));
	if (!block)
		return (NULL);
	ft_memmove(block, s, count);
	block[count] = '\0';
	return (block);
}

char	**ft_split(const char *s, char c)
{
	char	**mem;
	int		i;
	int		blockcount;

	if (!s || *s == '\0')
		return (NULL);
	blockcount = ft_blockcounter(s, c);
	mem = (char **)ft_calloc(sizeof(char *), (blockcount + 1));
	if (!mem)
		return (NULL);
	i = 0;
	while (i < (blockcount))
	{
		s = ft_skipper(s, c);
		mem[i] = allocate_block(s, c);
		if (!mem[i])
			return (ft_free(mem, i));
		s += ft_strlen(mem[i++]);
	}
	mem[i] = NULL;
	return ((char **)mem);
}
// would put 'ft_memcpy' because there is no memory overlap
// in the first place.
// int	main()
// {
// 	char	*str = "ab__42cd__dfgdfg";
// 	char	c = '_';
// 	char	**new = ft_split(str, c);
// 	// char	**new = ft_split(NULL, ' ');
// 	if (!new)
// 		printf("the result is NULL\n");
// 	// ft_split("   ", ' ');
// 	// ft_split("", ' ');
// 	else{
// 	int i = 0;
// 	while (new[i] != NULL)
// 	{
// 		//printf("bad\n");
// 		printf("My token %d: %s \n", i, new[i]);

// 		free(new[i]);
// 		i++;
// 	}
// 	}
// 	//printf("\nyousuck; the output should be {ab, cd, ef}");
// 	free (new);
// 	new = NULL;

// 	return (0);
// }
