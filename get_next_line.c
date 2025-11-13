/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malkilan <malkilan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:59:40 by malkilan          #+#    #+#             */
/*   Updated: 2025/11/13 12:56:23 by malkilan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen(const char *str)
{
	size_t	len;

	len = 0;
	while (str[len] != '\0')
		len++;
	return (len);
}

void	*free_and_return(char **stash)
{
	if (*stash)
	{
		free(*stash);
		*stash = NULL;
	}
	return (NULL);
}

char	*extract_line_from_stash(char **stash)
{
	char	*line;
	char	*newline_ptr;
	char	*new_stash;
	size_t	line_len;

	if (!stash || !*stash || **stash == '\0')
		return (NULL);
	newline_ptr = ft_strchr(*stash, '\n');
	if (newline_ptr)
	{
		line_len = (newline_ptr - *stash) + 1;
		line = ft_substr(*stash, 0, line_len);
		new_stash = ft_strdup(*stash + line_len);
		free(*stash);
		*stash = new_stash;
		if (!*stash)
			free(new_stash);
		return (line);
	}
	line = ft_strdup(*stash);
	free(*stash);
	*stash = NULL;
	return (line);
}

char	*read_to_stash(int fd, char *stash)
{
	char	*buffer;
	char	*temp;
	int		bytes_read;

	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (free_and_return(&stash));
	bytes_read = 1;
	while (bytes_read > 0 && !ft_strchr(stash, '\n'))
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read < 0)
		{
			free(buffer);
			free(stash);
			return (NULL);
		}
		buffer[bytes_read] = '\0';
		temp = ft_strjoin(stash, buffer);
		free(stash);
		stash = temp;
	}
	free(buffer);
	return (stash);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (free_and_return(&stash));
	stash = read_to_stash(fd, stash);
	if (!stash || !*stash)
		return (free_and_return(&stash));
	line = extract_line_from_stash(&stash);
	if (!line)
	{
		free(stash);
		stash = NULL;
	}
	return (line);
}
