/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malkilan <malkilan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:59:40 by malkilan          #+#    #+#             */
/*   Updated: 2025/09/09 19:04:18 by malkilan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"


char *get_next_line(int fd)
{
    size_t read_bytes;
    char *line;

    if (fd < 0 || BUFFER_SIZE <= 0)
        return (NULL);

    line = malloc(BUFFER_SIZE + 1);
    if (!line)
        return (NULL);
    while(!ft_strchr(line, '\n'))
    {
        read_bytes = read(fd, line, BUFFER_SIZE);
    }
}
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h> 
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
        printf("%s", line); // GNL returns line including '\n'
        free(line);         // always free the line after using it
    }

    close(fd);
    return (0);
}