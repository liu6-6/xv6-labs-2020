#include "kernel/types.h"
#include "user/user.h"

void newProcess(int p[]);

int main(int argc, char* argv[]) {
    
    int p[2];
    pipe(p);
    
    int id = fork();
    if (id > 0) {
        close(p[0]);
        for (int i = 2; i <= 35; i++) {
            write(p[1], &i, 4);
        }
        close(p[1]);
        wait((int *) 0);
    }
    else {
        newProcess(p);


    }

    exit(0);
}

void newProcess(int p[]) {
    close(p[1]);

    int first;
    if (read(p[0], &first, 4)) {
        // printf("%d\n", getpid());
        printf("prime %d\n", first);
    }
    else {
        exit(0);
    }

    int next;
    int p_next[2];
    pipe(p_next);

    int id = fork();

    if (id > 0) {
        close(p_next[0]);
        while (read(p[0], &next, 4)) {
            if (next % first != 0) {
                write(p_next[1], &next, 4);
            }
        }
        close(p_next[1]);
        wait((int *) 0);
    }
    else {
        newProcess(p_next);
    }

    exit(0);

    

}