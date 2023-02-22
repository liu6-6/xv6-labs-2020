#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char* dir, char* filename) {
    char buff[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(dir, 0)) < 0) {
        fprintf(2, "cant open %s\n", dir);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "cant stat %s\n", dir);
        close(fd);
        return;
    }

    // printf("23\n");

    switch (st.type) {
        case T_FILE:
            // printf("FILE\n");
            p = dir + strlen(dir) - 1;
            while (*p != '/') p--;

            if (strcmp(p + 1, filename) == 0) {
                fprintf(1, "%s\n", dir);
            }
            close(fd);
            break;

        case T_DIR:
            // printf("DIR\n");
            strcpy(buff, dir);
            p = buff + strlen(buff);
            *p++ = '/';
            while (read(fd, &de, sizeof(de))) {
                if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
                    continue;

                memmove(p, de.name, sizeof(de.name));
                buff[strlen(buff)] = '\0';
                // printf("%s\n", de.name);
                // printf("de.inum = %d\n", de.inum);
                // printf("%s\n", buff);
                find(buff, filename);
            }
            close(fd);
        break;
    }

    // printf("49\n");
    return;
}

int main(int argc, char* argv[]) {
    if (argc == 3) {
        find(argv[1], argv[2]);
    }

    exit(0);
}