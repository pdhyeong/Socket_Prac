#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_TRHEAD 2

pthread_mutex_t mutex;
int num = 0;
void *thread_inc(void *arg){
    int i;
    pthread_mutex_lock(&mutex);
    for(i = 0;i<5000;i++){
        num++;
    }
    pthread_mutex_unlock(&mutex);
    return NULL;
}
void *thread_des(void *arg){
    int i;
    pthread_mutex_lock(&mutex);
    for(i = 0;i<5000;i++){
        num--;
    }
    pthread_mutex_unlock(&mutex);
    return NULL;
}
int main(int argc,char *argv[]){
    pthread_t thread_id[NUM_TRHEAD];
    int i;

    pthread_mutex_init(&mutex,NULL);

    for(i=0;i<NUM_TRHEAD;i++){
        if(i%2){
            pthread_create(&(thread_id[i]),NULL,thread_inc,NULL);
        }
        else{
            pthread_create(&(thread_id[i]),NULL,thread_des,NULL);
        }
    }
    printf("remain value = %d\n",num);
    pthread_mutex_destroy(&mutex);
    return 0;
}