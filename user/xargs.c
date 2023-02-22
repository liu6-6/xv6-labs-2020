#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
    // if (argc <= 1) {
    //     fprintf(2, "xagrs: lack argments\n");
    //     exit(1);
    // }

    int maxArgs = 32;
    int argLen = 100;
    char** originArgs = (char **) malloc(maxArgs * sizeof (char *));
    // for (int i = 0; i < maxArgs; i++) 
    //     originArgs[i] = (char*) malloc(argLen * sizeof(char));
    
    // printf("argc = %d\n", argc);
    for (int i = 1; i < argc; i++) {
        originArgs[i - 1] = (char*) malloc(argLen * sizeof(char));
        strcpy(originArgs[i - 1], argv[i]);//Is the last char of argv[i] '\0'?
    }
    // printf("originArgs[0] = %s\n", originArgs[0]);

    int ExtraArgsNum = 0;
    int maxExtraArgsStringLen = 1000;//set for me
    char maxExtraArgsString[maxExtraArgsStringLen];
    memset(maxExtraArgsString, 0, sizeof(maxExtraArgsString));
    // int l = read(0, maxExtraArgsString, maxExtraArgsStringLen);
    int l = read(0, maxExtraArgsString, maxExtraArgsStringLen);
    // printf("maxExtraArgsString = %s\n", maxExtraArgsString);
    // printf("l = %d\n", l);
    int exactExtraArgsStringLen = strlen(maxExtraArgsString);
    // printf("extact_l = %d\n", exactExtraArgsStringLen);
    // printf("string = %s\n", maxExtraArgsString);
    // if (!read(0, maxExtraArgsString, maxExtraArgsStringLen)) {
    if (!l) {
        fprintf(2, "xargs: lack argments\n");
        exit(1);
    }
    else {
        // int exactExtraArgsStringLen = strlen(maxExtraArgsString);
        int argsStringIdx = 0;
        int newArgs = 1;
        for (int i = 0; i < exactExtraArgsStringLen; i++) {
            // printf("exactExtraArgsString[%d]= %c\n", i, maxExtraArgsString[i]);
            if (maxExtraArgsString[i] == ' ') {
                originArgs[argc - 1 + ExtraArgsNum][argsStringIdx] = '\0';
                ExtraArgsNum++;
                argsStringIdx = 0;
                newArgs = 1;
            }
            else if (maxExtraArgsString[i] == '\n' || maxExtraArgsString[i] == '\0') {
                // printf("i = %d\n", i);
                // printf("ExtraArgsNum = %d\n", ExtraArgsNum);
                
                int pid = fork();
                if (pid == 0) {
                    // printf("argv[1] = %s\n", argv[1]);
                    // printf("pid = %d\n", pid);
                    // printf("cnt = %d\n", ++cnt);
                    exec(argv[1], originArgs);
                    exit(0);
                }
                else {
                    wait(0);
                }
                for (int j = 0; j <= ExtraArgsNum; j++) {
                    memset(originArgs[argc - 1 + j], 0, sizeof(originArgs[argc - 1 + j]));
                        //without this after malloc, the contant before free will come back again
                    // printf("arv[%d] = %s\n", j, originArgs[argc - 1 + j]);
                    free(originArgs[argc - 1 + j]);
                    originArgs[argc - 1 + j] = 0;
                    // printf("after free arv[%d] = %s\n", j, originArgs[argc - 1 + j]);
                }
                ExtraArgsNum = 0;
                argsStringIdx = 0;
                newArgs = 1;
            }
            else {
                if (newArgs) {
                    // printf("79: arv[] = %s\n", originArgs[argc - 1 + ExtraArgsNum]);
                    originArgs[argc - 1 + ExtraArgsNum] = (char*) malloc(argLen * sizeof (char));
                    // printf("80: arv[] = %s\n", originArgs[argc - 1 + ExtraArgsNum]);
                    newArgs = 0;
                }
                originArgs[argc - 1 + ExtraArgsNum][argsStringIdx++] = maxExtraArgsString[i];
                // printf("add : %c\n", maxExtraArgsString[i]);
                // printf("arv[] = %s\n", originArgs[argc - 1 + ExtraArgsNum]);
            }
        }
    }
    // printf("originArgs[1] = %s\n", originArgs[1]);
    // printf("originArgs[2] = %s\n", originArgs[2]);

    exit(0);

}