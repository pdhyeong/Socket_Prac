#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

int sum = 0;
void *thread_summation(void *arg){
    int start = ((int *)arg)[0];
    int end = ((int *)arg)[1];
    int i;
    for(i = start;i<=end;i++){
        sum += i;
    }
    return NULL;
}

int main(int argc,char *argv[]){
    pthread_t id_t1,id_t2;
    int range1[] = {1,5};
    int range2[] = {6,10};

    pthread_create(&id_t1,NULL,thread_summation,(void *)range1);
    pthread_create(&id_t2,NULL,thread_summation,(void *)range2);

    pthread_join(id_t1,NULL);
    pthread_join(id_t2,NULL);

    printf("sum = %d \n",sum);
    return 0;
}