#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include "get_next_line.h"

char    *get_next_line(int fd); // your function

// Global for simulating read errors (you can ignore if not needed)
int next_read_error = 0;

// simple colors
#define RED   "\x1b[31m"
#define GRN   "\x1b[32m"
#define YEL   "\x1b[33m"
#define NC    "\x1b[0m"

// Global test result
int res = 1;

// Compare helper
int str_equal(const char *s1, const char *s2) {
    if (!s1 && !s2)
        return 1;
    if (!s1 || !s2)
        return 0;
    return strcmp(s1, s2) == 0;
}

// test_gnl
void test_gnl(int fd, const char *expected) {
    char *line = get_next_line(fd);
    if (!str_equal(line, expected)) {
        printf(RED "FAIL" NC ": got [%s], expected [%s]\n",
               line ? line : "NULL",
               expected ? expected : "NULL");
        res = 0;
    } else {
        printf(GRN "OK" NC ": got [%s]\n", line ? line : "NULL");
    }
    free(line);
}

// create a file with given content
void create_test_file(const char *filename, const char *content) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("open");
        exit(1);
    }
    write(fd, content, strlen(content));
    close(fd);
}

// limit test: file content depends on encoding string
int test_limit(const char *encoding) {
    char filename[] = "limit_test.txt";
    char buffer[1024];
    int len = 0;
    int num = 0;

    // Parse encoding string (like "10\n8\n")
    for (int i = 0; encoding[i]; ) {
        if (encoding[i] >= '0' && encoding[i] <= '9') {
            num = 0;
            while (encoding[i] >= '0' && encoding[i] <= '9') {
                num = num * 10 + (encoding[i] - '0');
                i++;
            }
            memset(buffer + len, 'x', num);
            len += num;
        } else if (encoding[i] == '\n') {
            buffer[len++] = '\n';
            i++;
        } else {
            i++;
        }
    }
    buffer[len] = '\0';

    // Create file with content
    create_test_file(filename, buffer);

    // Test reading with gnl
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("open limit_test");
        return 0;
    }
    char *line;
    while ((line = get_next_line(fd)) != NULL) {
        printf("line: [%s]\n", line);
        free(line);
    }
    close(fd);
    return 1;
}

int main(void) {
    // ==== read_error.txt test ====
    create_test_file("read_error.txt", "aaaaaaaaaa\nbbbbbbbbbb\ncccccccccc\ndddddddddd\n");

    int fd = open("read_error.txt", O_RDONLY);
    test_gnl(fd, "aaaaaaaaaa\n");  // 1
    test_gnl(fd, "bbbbbbbbbb\n");  // 2

    // simulate read error
    next_read_error = 1;
    if (BUFFER_SIZE > 100) {
        char *temp;
        do {
            temp = get_next_line(fd);
            free(temp);
        } while (temp != NULL);
    }
    test_gnl(fd, NULL);  // 3
    next_read_error = 0;
    close(fd);

    // reopen file
    fd = open("read_error.txt", O_RDONLY);
    test_gnl(fd, "aaaaaaaaaa\n");  // 4
    test_gnl(fd, "bbbbbbbbbb\n");  // 5
    test_gnl(fd, "cccccccccc\n");  // 6
    test_gnl(fd, "dddddddddd\n");  // 7
    test_gnl(fd, NULL);            // 8
    close(fd);

    // ==== BUFFER_SIZE == 10 limit tests ====
    char *tests[18] = {
        "9", "9\n", "10", "10\n", "11", "11\n", "19", "19\n", "20", "20\n", "21", "21\n",
        "9\n9\n", "9\n10", "9\n10\n",
        "10\n8\n", "10\n9", "10\n9\n"
    };

    if (BUFFER_SIZE == 10) {
        for (int i = 0; i < 18; i++) {
            printf(YEL "== limit test: %s ==\n" NC, tests[i]);
            res = test_limit(tests[i]) && res;
        }
    }

    if (res)
        printf(GRN "All tests passed!\n" NC);
    else
        printf(RED "Some tests failed.\n" NC);

    return res ? 0 : 1;
}
