/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malkilan <malkilan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:59:40 by malkilan          #+#    #+#             */
/*   Updated: 2025/09/21 18:27:47 by malkilan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*extract_line_from_stash(char **stash)
{
	char	*line;
	char	*new_stash;
	char	*newline_ptr;
	int		line_len;

	newline_ptr = ft_strchr(*stash, '\n');
	if (newline_ptr)
	{
		line_len = newline_ptr - *stash + 1;
		line = ft_substr(*stash, 0, line_len);
		new_stash = ft_substr(*stash, line_len, ft_strlen(*stash) - line_len);
		free(*stash);
		*stash = new_stash;
		if (**stash == '\0')
		{
			free(*stash);
			*stash = NULL;
		}
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
		return (NULL);
	bytes_read = 1;
	while (bytes_read > 0 && !ft_strchr(stash, '\n'))
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read < 0)
		{
			free(buffer);
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
	static char	*stash[4096];
	char		*line;

	if (fd < 0 || read(fd, NULL, 0) < 0 || BUFFER_SIZE <= 0 || fd > 4096)
	{
		free(stash[fd]);
		stash[fd] = NULL;
		return (NULL);
	}
	if (!stash[fd])
		stash[fd] = ft_strdup("");
	stash[fd] = read_to_stash(fd, stash[fd]);
	if (!stash[fd] || !*stash[fd])
	{
		free(stash[fd]);
		stash[fd] = NULL;
		return (NULL);
	}
	line = extract_line_from_stash(&stash[fd]);
	return (line);
}
