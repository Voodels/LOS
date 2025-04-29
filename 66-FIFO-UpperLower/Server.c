#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <ctype.h>

#define REQ_FIFO "request.fifo"
#define RES_FIFO "response.fifo"

int main() {
    unlink(REQ_FIFO);
    unlink(RES_FIFO);
    mkfifo(REQ_FIFO, 0666);
    mkfifo(RES_FIFO, 0666);

    int req_fd = open(REQ_FIFO, O_RDONLY);
    int res_fd = open(RES_FIFO, O_WRONLY);

    char buf[1024];
    int n;
    while ((n = read(req_fd, buf, sizeof(buf))) > 0) {
        for (int i = 0; i < n; i++)
            buf[i] = tolower(buf[i]);
        write(res_fd, buf, n);
    }

    close(req_fd);
    close(res_fd);
    unlink(REQ_FIFO);
    unlink(RES_FIFO);
    return 0;
}