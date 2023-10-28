#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

void
filter(int readfd) {
    int p, n;
    if (!read(readfd, &p, sizeof(int))) return;
    printf("prime %d\n", p);
    
    int fds[2];
    pipe(fds);

    int pid;
    if ((pid = fork()) == 0) {
        close(readfd);
        close(fds[1]);
        filter(fds[0]);
        close(fds[0]);
    } else if (pid > 0) {
        close(fds[0]);
        while (read(readfd, &n, sizeof(int))) {
            if (n % p) {
                write(fds[1], &n, sizeof(int));
            }
        }
        close(readfd);
        close(fds[1]);
        wait(0);
    }
    exit(0);
}

void
main() {
    int fds[2];
    pipe(fds);
    for (int i = 2; i <= 35; i++) {
        write(fds[1], &i, sizeof(int));
    }
    close(fds[1]);
    filter(fds[0]);
}
