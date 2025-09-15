/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malkilan <malkilan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:59:40 by malkilan          #+#    #+#             */
/*   Updated: 2025/09/15 15:47:31 by malkilan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static void cleanup_and_return(char **stash, char *buffer)
{
    if (buffer)
        free(buffer);
    if (stash && *stash)
    {
        free(*stash);
        *stash = NULL;
    }
}

static int read_chunk(int fd, char *buffer, char **stash)
{
    char    *tmp;
    int     bytes;

    bytes = read(fd, buffer, BUFFER_SIZE);
    if (bytes <= 0)
        return (bytes);
    buffer[bytes] = '\0';
    tmp = ft_strjoin(*stash, buffer);
    if (!tmp)
        return (-1);
    free(*stash);
    *stash = tmp;
    return (bytes);
}

static char *read_and_stash(int fd, char **stash)
{
    char    *buffer;
    int     result;

    buffer = malloc(BUFFER_SIZE + 1);
    if (!buffer)
        return (NULL);
    result = 1;
    while (result > 0 && find_newline(*stash) == -1)
    {
        result = read_chunk(fd, buffer, stash);
        if (result < 0)
        {
            cleanup_and_return(stash, buffer);
            return (NULL);
        }
    }
    free(buffer);
    return (*stash);
}

static char *extract_line(char **stash)
{
    char    *line;
    char    *tmp;
    int     nl_pos;

    if (!*stash || **stash == '\0')
        return (NULL);
    nl_pos = find_newline(*stash);
    if (nl_pos >= 0)
    {
        line = ft_substr(*stash, 0, nl_pos + 1);
        tmp = ft_substr(*stash, nl_pos + 1, ft_strlen(*stash) - nl_pos - 1);
        free(*stash);
        *stash = tmp;
    }
    else
    {
        line = ft_strdup(*stash);
        free(*stash);
        *stash = NULL;
    }
    return (line);
}

char    *get_next_line(int fd)
{
    static char *stash;
    char        *line;

    if (fd < 0 || BUFFER_SIZE <= 0)
        return (NULL);
    if (!stash)
    {
        stash = ft_strdup("");
        if (!stash)
            return (NULL);
    }
    if (!read_and_stash(fd, &stash))
        return (NULL);
    line = extract_line(&stash);
    if (!line && stash)
    {
        free(stash);
        stash = NULL;
    }
    return (line);
}


/* #include <fcntl.h>
#include <stdlib.h>

int	main(void)
{
    int   fd;
    char *line;

   fd = open("test.txt", O_RDONLY);
    if (fd < 0)
    {
        perror("Error opening file");
        return (1);
    }

        line = get_next_line(fd);
        printf("%s", line);
        free(line);
    // while ((line = get_next_line(fd)) != NULL)
    // {
    //     printf("%s", line);
    //     free(line);
    // }

   close(fd);
    return (0);
} */