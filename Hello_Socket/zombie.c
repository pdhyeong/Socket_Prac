#include <stdio.h>
#include <unistd.h>

int main(int argc,char *argv[]){
    pid_t pid = fork();

    if(pid == 0){
        puts("Child Process");
    }
    else{
        printf("Chlid Process Id : %d ",pid);
        sleep(30);
    }
    if(pid == 0) puts("End Child Process");
    else puts("End parent Process");
    return 0;
}