#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int
main(int argc, char *argv[]) {
    int p[2];

    if (pipe(p) < 0) {
        fprintf(2, "pipe\n");
        exit(1);
    }

    int pid;
    char buf[1];
    if ((pid = fork()) == 0) {
        read(p[0], buf, 1);
        printf("%d: received ping\n", getpid());
        close(p[0]);
        write(p[1], buf, 1);
        close(p[1]);
    } else if (pid > 0) {
        write(p[1], buf, 1);
        close(p[1]);
        read(p[0], buf, 1);
        printf("%d: received pong\n", getpid());
        close(p[0]);
    } else {
        fprintf(2, "fork\n");
        exit(1);
    }
    exit(0);
}
