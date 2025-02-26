/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamaien <zjamaien@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 21:52:37 by zjamaien          #+#    #+#             */
/*   Updated: 2025/02/25 22:46:47 by zjamaien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h" 

char	*ft_strtok(char *str, const char *delim)
{
	static char	*stock;
	char		*ptr;

	ptr = NULL;
	if (str != NULL)
		stock = str;
	if (stock == NULL)
		return (NULL);
	while (*stock && ft_strchr(delim, *stock))
		stock++;
	if (*stock == '\0')
		return (NULL);
	ptr = stock;
	while (*stock && !ft_strchr(delim, *stock))
		stock++;
	if (*stock)
	{
		*stock = '\0';
		stock++;
	}
	return (ptr);
}
/*
int	main()
{
	char str[] = "zaid is best man ever";
	char *tok;
	tok = strtok(str, " ");
	while (tok != NULL)
	{
		printf("%s\n", tok);
		tok = strtok(NULL, " ");
	}
	return (0);
}
*/
