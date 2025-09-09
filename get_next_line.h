/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malkilan <malkilan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 18:25:01 by malkilan          #+#    #+#             */
/*   Updated: 2025/09/09 19:05:45 by malkilan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
#ifndef BUFFER_SIZE
#   define BUFFER_SIZE 1
#endif
# include <stdlib.h>
# include <unistd.h>

char *get_next_line(int fd);
static char	*ft_strchr(const char *s, int c);
static char	*ft_strdup(const char *s);
static char	*ft_strjoin(char const *s1, char const *s2);
static size_t	ft_strlen(const char *str);
#endif