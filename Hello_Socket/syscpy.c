#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BUF_SIZE 3

int main(int argc, char *argv[])
{
    int fd1, fd2, len;
    char buf[BUF_SIZE];

    fd1 = open("news.txt", O_RDONLY);
    if (fd1 == -1) {
        perror("Error opening news.txt");
        return 1;
    }

    fd2 = open("cpy.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd2 == -1) {
        perror("Error opening cpy.txt");
        close(fd1);
        return 1;
    }

    while ((len = read(fd1, buf, sizeof(buf))) > 0) {
        if (write(fd2, buf, len) != len) {
            perror("Error writing to cpy.txt");
            close(fd1);
            close(fd2);
            return 1;
        }
    }

    if (len == -1) {
        perror("Error reading from news.txt");
    }

    close(fd1);
    close(fd2);
    return 0;
}
