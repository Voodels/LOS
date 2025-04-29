#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define REQ_FIFO "request.fifo"
#define RES_FIFO "response.fifo"

int main() {
    int req_fd = open(REQ_FIFO, O_WRONLY);
    int res_fd = open(RES_FIFO, O_RDONLY);

    char *msg = "HELLO WORLD";
    write(req_fd, msg, 11);

    char buf[1024];
    int n = read(res_fd, buf, sizeof(buf));
    buf[n] = '\0';
    printf("Converted: %s\n", buf);

    close(req_fd);
    close(res_fd);
    return 0;
}