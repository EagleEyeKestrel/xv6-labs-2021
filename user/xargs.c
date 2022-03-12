#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int
main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(2, "Usage: xargs [command]...\n");
        exit(1);
    }
    if (argc > MAXARG) {
        fprintf(2, "xargs: too many arguments\n");
        exit(1);
    }
    char buf[512], ch;
    char *xargs[MAXARG];
    int now = 0;
    while (read(0, &ch, 1) == 1) {
        buf[now++] = ch;
    }
    for (int i = 1; i < argc; i++) {
        xargs[i - 1] = argv[i];
    }
    char *p = buf;
    for (int i = 0; i < 512; i++) {
        if (buf[i] == '\n') {
            if (fork() > 0) {
                p = buf + i + 1;
                wait(0);
            } else {
                buf[i] = 0;
                xargs[argc - 1] = p;
                xargs[argc] = 0;
                exec(xargs[0], xargs);
                exit(1);
            }
        }
    }
    exit(0);
}