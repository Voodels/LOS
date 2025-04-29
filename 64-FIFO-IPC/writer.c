#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main() {
    mkfifo("myfifo", 0666);
    int fd = open("myfifo", O_WRONLY);
    printf("Writer opened FIFO\n");
    write(fd, "Hello from writer!", 17);
    close(fd);
    return 0;
}