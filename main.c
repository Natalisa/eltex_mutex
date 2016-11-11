#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#define count_shop 5
#define count_client 2

pthread_mutex_t mutex[count_shop];

int shop[count_shop];

struct a{
    int name;
    int amt;
} client[count_client];

void* thread_func(void *arg){
    int cl = *((int*)arg);
    while(1){
        int index = rand() % count_shop;
        if(pthread_mutex_trylock(&mutex[index]) == 0 && shop[index] > 0){
            shop[index]-=client[cl].amt;
            if(shop[index] < 0) {
                client[cl].amt = abs(shop[index]);
                shop[index] = 0;
            } else client[cl].amt = 0;
            printf("К %d м %d ост %d\n", cl,index,shop[index]);
            pthread_mutex_unlock(&mutex[index]);
            if(client[cl].amt == 0){
                client[cl].amt = rand()%20+10;
                // printf("Клиент %d хочет %d\n", cl, client[cl].amt);
            }
        }
        sleep(rand()%3);
    }
}

int main(){
    pthread_t thread[count_client];
    int truck = 50;
    int i;
    for (i=0; i < count_shop; i++){
        shop[i] = 50;
    }
    for(i = 0; i < count_client; i++){
        client[i].name = i;
        client[i].amt = 20;
        pthread_create(&thread[i], NULL, thread_func, (void*)(&client[i].name));
    }
    while(1){
        int index = rand() % count_shop;
        if(pthread_mutex_trylock(&mutex[index]) == 0 && shop[index] == 0){
            shop[index]+=truck;
            printf("В магазине %d кол товара %d\n",index,shop[index]);
            pthread_mutex_unlock(&mutex[index]);
        }
        sleep(rand()%2+1);
    }
    return 0;
}
