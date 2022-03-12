#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
lastFileName(char *path)
{
  static char buf[DIRSIZ + 1];
  char *p;

  // Find first character after last slash.
  for (p = path + strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if (strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  *(buf + strlen(p)) = '\0';
  return buf;
}

void
find(char *dir_name, char *file_name) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;
    if ((fd = open(dir_name, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", dir_name);
        exit(1);
    }
    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", dir_name);
        exit(1);
    }
    switch (st.type) {
        case T_FILE:
            if (!strcmp(lastFileName(dir_name), file_name)) {
                printf("%s\n", dir_name);
            }
            break;
        case T_DIR:
            if (strlen(dir_name) + 1 + DIRSIZ + 1 > sizeof buf) {
                printf("find: path too long\n");
                break;
            }
            strcpy(buf, dir_name);
            p = buf + strlen(buf);
            *p++ = '/';
            while (read(fd, &de, sizeof(de)) == sizeof(de)) {
                if (de.inum == 0) continue;
                if (!strcmp(de.name, ".") || !strcmp(de.name, "..")) continue;
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                find(buf, file_name);
            }
            break;
    }
    close(fd);
}

int
main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(2, "Usage: find dir filename...\n");
        exit(1);
    }
    char *dir_name = argv[1];
    char *file_name = argv[2];
    find(dir_name, file_name);
    exit(0);
}