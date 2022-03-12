#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[]) {
    int p[2];
    pipe(p);
    int pid;
    if (fork() == 0) {
        pid = getpid();
        char tmp[1];
        read(p[0], tmp, 1);
        printf("%d: received ping\n", pid);
        close(p[0]);
        write(p[1], tmp, 1);
        close(p[1]);
        exit(0);
    } else {
        pid = getpid();
        char toSend[1] = {'0'};
        write(p[1], toSend, 1);
        close(p[1]);
        wait(0);
        char tmp[1];
        read(p[0], tmp, 1);
        printf("%d: received pong\n", pid);
        close(p[0]);
        exit(0);
    }
}