#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <pthread.h>
#include <stdio.h>
#include "request.h"
#include "dlist.c"

#define TRUE 1
#define FALSE 0
#define MAXSIZE 256

void process_message(struct request *msg);

pthread_mutex_t mutex_msg;
int msg_not_copied = TRUE;
pthread_cond_t cond_msg;


int set_value(int key, char *value1, float value2) {
    Node* newNode = getNewNode(key, value1, value2);
    int result = insert(newNode);
    printf("CODE FROM INSERT_:_:_:_:_:_:_:_%d", result);
    return result;
}

Node* get_value(int key) {
    return search(key);
}

int modify_value(int key, char *value1, float value2) {
    Node* newNode = getNewNode(key, value1, value2);
    return modify(newNode);
}

int delete_key(int key) {
    return delete(key);
}

int num_items() {
    return getCardinality(head);
}

int main(int argc, char* argv[]) {
    mqd_t q_server;         /* server queue */
    struct request msg;     /* message to receive */
    struct mq_attr q_attr;  /* queue attributes */
    pthread_attr_t t_attr;  /* thread attributes */

    q_attr.mq_maxmsg = 10;
    q_attr.mq_msgsize = sizeof(struct request);

    q_server = mq_open("/SERVERR", O_CREAT|O_RDONLY, 0666, &q_attr);
    if (q_server == -1) {
        perror("Can't create server queue");
        return -1;
    }

    pthread_mutex_init(&mutex_msg, NULL);
    pthread_cond_init(&cond_msg, NULL);
    pthread_attr_init(&t_attr);

    /* thread attributes */
    pthread_attr_setdetachstate(&t_attr, PTHREAD_CREATE_DETACHED);

    pthread_t thr;

    while(TRUE) {
        mq_receive(q_server, (char*) &msg, sizeof(struct request), 0);
        perror("mqreceive info");

        pthread_create(&thr, &t_attr, (void*)process_message, &msg);

        /* wait for thread to copy message */
        pthread_mutex_lock(&mutex_msg);
        while (msg_not_copied) {
            pthread_cond_wait(&cond_msg, &mutex_msg);
        }
        msg_not_copied = TRUE;
        pthread_mutex_unlock(&mutex_msg);
    }
}

int getResponse(struct request* localreq) {
    switch(localreq->id_method) {
        case 0:
            return init();
        case 1:
            return set_value(localreq->key, localreq->value1, localreq->value2);
        case 2:
            ;
            Node* temp = get_value(localreq->key);
            if(temp == NULL) {
		            printf("ERROR-------------------%d\n", localreq->key);
                return -1;
            }
            strcpy(localreq->value1, temp->value1);
            localreq->value2 = temp->value2;
            return 0;
        case 3:
            return modify_value(localreq->key, localreq->value1, localreq->value2);
        case 4:
            return delete_key(localreq->key);
        case 5:
            return getCardinality(head);
        default:
		printf("NO METHOD RECOGNIZED");
            return -1;
    }
}

void process_message(struct request *msg) {
    if(head != NULL)
    printf("CURRENT KEY OF HEAD:::: %d\n\n", head->key);
    printList();
    struct request msg_local;   /* local message */
    mqd_t q_client;      /* client queue */

    /* thread copies message to local message */
    pthread_mutex_lock(&mutex_msg);
    memcpy(&msg_local, msg, sizeof(struct request));

    /* wake up server */
    msg_not_copied = FALSE;

    pthread_cond_signal(&cond_msg);

    pthread_mutex_unlock(&mutex_msg);

    /* execute client request and prepare reply */

    printf("Received request from: %s\n", (char*)msg_local.q_name);
    printf("MESSAGE RECEIVED\n");
    printf("id_method: %d\n", msg_local.id_method);
    printf("key: %d\n", msg_local.key);
    printf("value1: %s\n", msg_local.value1);
    printf("value2: %f\n", msg_local.value2);

    /* return result to client by sending it to queue */
    q_client = mq_open(msg_local.q_name, O_WRONLY);

    int result = getResponse(&msg_local);

    msg_local.id_method = result;

    printf("Code from server: %d, %d\n", result, msg_local.id_method);

    printf("MESSAGE SENT\n");
    printf("id_method: %d\n", msg_local.id_method);
    printf("key: %d\n", msg_local.key);
    printf("value1: %s\n", msg_local.value1);
    printf("value2: %f\n", msg_local.value2);

    if (q_client == -1)
        perror("Can't open client queue");
    else {
        mq_send(q_client, (const char*) &msg_local, sizeof(struct request), 0);
        printf("Msg sent\n");

        mq_close(q_client);
        printf("Queue closed\n");
        printf("********************\n");
    }
    pthread_exit(0);
}
