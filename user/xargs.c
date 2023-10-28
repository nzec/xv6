#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int
main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(2, "usage: %s [cmds..]\n", argv[0]);
        exit(1);
    }
    
    char *rargv[argc + 1];
    char buf[512];
    char *p = buf;

    while (read(0, p, 1)) {
        if (*p == '\n') {
            *p = '\0';
            for (int i = 1; i < argc; i++) {
                rargv[i - 1] = argv[i];
            }
            rargv[argc - 1] = buf;
            rargv[argc] = 0;
            p = buf;
            if (fork() == 0) {
                exec(rargv[0], rargv);
            }
        }
        p++;
    }

    exit(0);
}
