#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define TABLESIZE 36

void
f(int read_fd, int write_fd) {
    int isprime[TABLESIZE];
    read(read_fd, isprime, TABLESIZE * sizeof(int));
    int p = -1;
    for (int i = 0; i < TABLESIZE; i++) {
        if (isprime[i] == 1) {
            p = i;
            break;
        }
    }
    if (p == -1) {
        return;
    }
    printf("prime %d\n", p);
    for (int j = p; j < TABLESIZE; j += p) {
        isprime[j] = 0;
    }
    if (fork() > 0) {
        close(read_fd);
        write(write_fd, isprime, TABLESIZE * sizeof(int));
        wait(0);
        return;
    } else {
        f(read_fd, write_fd);
    }
}

int
main(int argc, char *argv[]) {
    int p[2];
    pipe(p);
    int isprime[TABLESIZE];
    isprime[0] = 0;
    isprime[1] = 0;
    for (int i = 2; i < TABLESIZE; i++) {
        isprime[i] = 1;
    }
    if (fork() > 0) {
        close(p[0]);
        write(p[1], isprime, TABLESIZE * sizeof(int));
        wait(0);
        exit(0);
    } else {
        f(p[0], p[1]);
        exit(0);
    }
}