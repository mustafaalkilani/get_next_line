/* get_next_line.c - single-file implementation (no external utils) */
#include <unistd.h>
#include <stdlib.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 42
#endif

/* ------------------ helpers ------------------ */
static size_t  gnl_strlen(const char *s)
{
    size_t i = 0;
    if (!s) return 0;
    while (s[i]) i++;
    return i;
}

static char *gnl_strchr(const char *s, int c)
{
    if (!s) return NULL;
    while (*s)
    {
        if (*s == (char)c) return (char *)s;
        s++;
    }
    if (c == 0) return (char *)s;
    return NULL;
}

/* join s1 + s2, free s1 (convenient for building leftover) */
static char *gnl_strjoin_and_free(char *s1, const char *s2)
{
    size_t l1 = gnl_strlen(s1);
    size_t l2 = gnl_strlen(s2);
    char *res = malloc(l1 + l2 + 1);
    size_t i = 0;

    if (!res) { free(s1); return NULL; }
    while (i < l1) { res[i] = s1[i]; i++; }
    for (size_t j = 0; j < l2; ++j) res[i++] = s2[j];
    res[i] = '\0';
    free(s1);
    return res;
}

static char *gnl_substr(const char *s, unsigned int start, size_t len)
{
    size_t sl = gnl_strlen(s);
    size_t i = 0;
    char *res;

    if (!s || start >= sl) return NULL;
    if (len > sl - start) len = sl - start;
    res = malloc(len + 1);
    if (!res) return NULL;
    while (i < len) { res[i] = s[start + i]; i++; }
    res[i] = '\0';
    return res;
}

static char *gnl_strdup(const char *s)
{
    size_t l = gnl_strlen(s);
    char *d = malloc(l + 1);
    if (!d) return NULL;
    for (size_t i = 0; i < l; ++i) d[i] = s[i];
    d[l] = '\0';
    return d;
}

/* ------------------ get_next_line ------------------ */
char *get_next_line(int fd)
{
    static char *leftover;          /* saved between calls */
    char *buffer;
    ssize_t bytes_read;
    char *newline_pos;
    char *line;

    if (fd < 0 || BUFFER_SIZE <= 0)
        return NULL;

    buffer = malloc(BUFFER_SIZE + 1);
    if (!buffer) return NULL;

    /* read until we find a newline in leftover or EOF/error */
    while (!gnl_strchr(leftover, '\n') &&
           (bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0)
    {
        buffer[bytes_read] = '\0';
        leftover = gnl_strjoin_and_free(leftover, buffer);
        if (!leftover) { free(buffer); return NULL; }
    }
    free(buffer);

    /* read error */
    if (bytes_read < 0)
    {
        if (leftover) { free(leftover); leftover = NULL; }
        return NULL;
    }

    /* nothing left */
    if (!leftover || *leftover == '\0')
    {
        if (leftover) { free(leftover); leftover = NULL; }
        return NULL;
    }

    /* if newline found, split leftover into line + new leftover */
    newline_pos = gnl_strchr(leftover, '\n');
    if (newline_pos)
    {
        size_t idx = (size_t)(newline_pos - leftover);
        line = gnl_substr(leftover, 0, idx + 1); /* include '\n' */
        /* remainder after newline */
        char *new_left = gnl_substr(leftover, idx + 1, gnl_strlen(leftover) - idx - 1);
        free(leftover);
        leftover = new_left; /* may be NULL or empty string */
        return line;
    }

    /* no newline -> return what's left (last line without '\n') */
    line = gnl_strdup(leftover);
    free(leftover);
    leftover = NULL;
    return line;
}
