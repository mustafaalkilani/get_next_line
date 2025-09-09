/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malkilan <malkilan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:59:40 by malkilan          #+#    #+#             */
/*   Updated: 2025/09/09 19:34:04 by malkilan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h> 

char *get_next_line(int fd)
{
    size_t  read_bytes;
    char *buffer;
    static char *stash;
    int i = 0;
    if (fd < 0 || BUFFER_SIZE <= 0)
        return (NULL);

    buffer = malloc(BUFFER_SIZE + 1);
    if (!buffer)
        return (NULL);

    read_bytes = 1;
    while (!ft_strchr(stash, '\n') && read_bytes > 0)
    {
        read_bytes = read(fd, buffer, BUFFER_SIZE);
        if (read_bytes < 0)
        {
            free(buffer);
            free(stash);
        }
        i++;
    buffer[read_bytes] = '\0';
    stash = ft_strjoin(stash, buffer);
    }
    if (ft_strchr(stash, '\n'))
    {
        buffer = ft_substr(stash, 0, i);
        stash = ft_substr(stash, i, ft_strlen(stash));
        return buffer
    }
    free(buffer);
    return (stash);
}
#include <fcntl.h>
#include <stdlib.h>
int main(void)
{
    int   fd;
    char *line;

    fd = open("test.txt", O_RDONLY);
    if (fd < 0)
    {
        perror("Error opening file");
        return (1);
    }

    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }

    close(fd);
    return (0);
}