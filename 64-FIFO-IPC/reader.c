#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd = open("myfifo", O_RDONLY);
    printf("Reader opened FIFO\n");
    char buf[100];
    int n = read(fd, buf, sizeof(buf));
    buf[n] = '\0';
    printf("Received: %s\n", buf);
    close(fd);
    return 0;
}