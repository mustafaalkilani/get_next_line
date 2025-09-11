/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malkilan <malkilan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:59:40 by malkilan          #+#    #+#             */
/*   Updated: 2025/09/11 16:12:16 by malkilan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h> 

static void replace_stash(char **stash, char *new_stash)
{
    free(*stash);
    *stash = new_stash;
}

int check_nl(char *buffer)
{
    int i = 0;
    if (!buffer)
        return (-1);
    while (buffer[i])
    {
        if (buffer[i] == '\n')
            return (i);
        i++;
    }
    return (-1);
}

char *get_next_line(int fd)
{
    char        *buffer;
    int         read_bytes;
    static char *stash;
    int         nl_index;
    char        *line;
    char        *tmp;

    if (fd < 0 || BUFFER_SIZE <= 0)
        return (NULL);
    buffer = malloc(BUFFER_SIZE + 1);
    if (!buffer)
        return (NULL);
    if (!stash)
        stash = ft_strdup("");
    read_bytes = 1;
    while (read_bytes > 0 && check_nl(stash) == -1)
    {
        read_bytes = read(fd, buffer, BUFFER_SIZE);
        if (read_bytes <= 0)
            break ;
        buffer[read_bytes] = '\0';
        tmp = ft_strjoin(stash, buffer);
        replace_stash(&stash, tmp);
    }
    free(buffer);

    if (!stash || *stash == '\0')
    {
        free(stash);
        stash = NULL;
        return (NULL);
    }
    nl_index = check_nl(stash);
    if (nl_index >= 0)
    {
        line = ft_substr(stash, 0, nl_index + 1);
        tmp = ft_substr(stash, nl_index + 1, ft_strlen(stash) - nl_index - 1);
        replace_stash(&stash, tmp);
    }
    else
    {
        line = ft_strdup(stash);
        free(stash);
        stash = NULL;
    }
    return (line);
}

// #include <fcntl.h>
// #include <stdlib.h>
// int main(void)
// {
//     int   fd;
//     char *line;

//    fd = open("test.txt", O_RDONLY);
//     if (fd < 0)
//     {
//         perror("Error opening file");
//         return (1);
//     }

//     while ((line = get_next_line(fd)) != NULL)
//     {
//         printf("%s", line);
//         free(line);
//     }

//    close(fd);
//     return (0);
// }