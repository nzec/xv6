#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

char buf[512];
void
find(char *haystack, char *needle) {
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(haystack, O_RDONLY)) < 0) {
        fprintf(2, "find: cannot open %s %d\n", haystack, strlen(haystack));
        return;
    }
    if (fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", haystack);
        close(fd);
        return;
    }

    char *p;
    for (p = haystack + strlen(haystack); p >= haystack && *p != '/'; p--);
    p++;

    if (strcmp(p, needle) == 0) {
        printf("%s\n", haystack);
    }

    if (st.type == T_DIR) {
        while (read(fd, &de, sizeof(de)) == sizeof(de)) {
            if (de.inum == 0 || de.name[0] == '.') continue;
            int n = strlen(haystack);
            *(haystack + n) = '/';
            strcpy(haystack + n + 1, de.name);

            find(haystack, needle);
            haystack[n] = '\0';
        }
    }
    close(fd);
}

int
main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(2, "usage: %s [haystack] [needle]\n", argv[0]);
        exit(1);
    }
    strcpy(buf, argv[1]);
    find(argv[1], argv[2]);
    exit(0);
}
