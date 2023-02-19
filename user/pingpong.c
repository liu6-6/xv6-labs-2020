#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]){
    int p[2];
    char parent_received;
    char child_received;
    pipe(p);
    int p1[2];
    pipe(p1);

    int id = fork();
    if(id != 0){
       //nt pid = getpid();
       char byte = 'a';
       close(p[0]);
       write(p[1], &byte, 1);
       close(p[1]);
       
    //    read(p[0], &parent_received, 1);
    //    printf("%d: received pong\n", pid);
    //    close(p[0]); 
    }
    else{
        int pid = getpid();
        close(p[1]);
        read(p[0], &child_received, 1);
        printf("%d: received ping\n", pid);
        //fflush(stdout);
        close(p[0]);
        
        // write(p[1], &child_received, 1);
        // close(p[1]);
    }
    
    if(id != 0){
       int pid = getpid();
       close(p1[1]); 
       read(p1[0], &parent_received, 1);
       printf("%d: received pong\n", pid);
       //fflush(stdout);
       close(p1[0]);
       
    }
    else{
        close(p1[0]);
        write(p1[1], &child_received, 1);
        close(p1[1]);
        
    }
    

    exit(0);
        

    
}