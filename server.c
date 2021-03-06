/****************************************************************************
 * server.c
 *    Implementation of server main part of receiving and sending messages.
 * Author: Guillermo Escobero
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <pthread.h>

#include "message.h"
#include "dlist.c"

#define TRUE 1
#define FALSE 0
#define MAXSIZE 256

#define SERVER_NAME "/SERVER_01"

void process_message(struct message *msg);

pthread_mutex_t mutex_msg;
int msg_not_copied = TRUE;
pthread_cond_t cond_msg;

int main(int argc, char* argv[]) {
    printf("Starting server...\n");
    mqd_t q_server;         /* Server queue */
    struct message msg;     /* Message to receive */
    struct mq_attr q_attr;  /* Queue attributes */
    pthread_attr_t t_attr;  /* Thread attributes */

    q_attr.mq_maxmsg = 10;
    q_attr.mq_msgsize = sizeof(struct message);

    q_server = mq_open(SERVER_NAME, O_CREAT|O_RDONLY, 0666, &q_attr);
    if (q_server == -1) {
        perror("Can't create server queue");
        return -1;
    } else {
        printf("Server up\n");
    }

    pthread_mutex_init(&mutex_msg, NULL);
    pthread_cond_init(&cond_msg, NULL);
    pthread_attr_init(&t_attr);

    /* Thread attributes */
    pthread_attr_setdetachstate(&t_attr, PTHREAD_CREATE_DETACHED);

    pthread_t thr;

    while(TRUE) {
        printf("\nWaiting for clients...\n");
        if(mq_receive(q_server, (char*) &msg, sizeof(struct message), 0) == -1) {
            perror("MQRECEIVE ERROR");
        }


        if(pthread_create(&thr, &t_attr, (void*)process_message, &msg) != 0) {
            perror("SERVER CAN'T CREATE THREADS");
            printf("FATAL ERROR");
            return -1;
        }

        /* Wait for thread to copy message */
        pthread_mutex_lock(&mutex_msg);
        while (msg_not_copied) {
            pthread_cond_wait(&cond_msg, &mutex_msg);
        }
        msg_not_copied = TRUE;
        pthread_mutex_unlock(&mutex_msg);
    }
}

int getResponse(struct message* localreq) {
    switch(localreq->id_method) {
        case 0:
            /* init() */
            return freeList();
        case 1:
            /* set_value() */
            ;
            Node* newNode = getNewNode(localreq->key, localreq->value1, localreq->value2);
            if(newNode == NULL) {
                return -1;
            }
            return insert(newNode);
        case 2:
            /* get_value() */
            ;
            Node* temp = search(localreq->key);
            if(temp == NULL) {
                return -1;
            }
            strcpy(localreq->value1, temp->value1);
            localreq->value2 = temp->value2;
            return 0;
        case 3:
            /* modify_value() */
            ;
            Node* newNode2 = getNewNode(localreq->key, localreq->value1, localreq->value2);
            if(newNode2 == NULL) {
                return -1;
            }
            return modify(newNode2);
        case 4:
            /* delete_key() */
            return delete(localreq->key);
        case 5:
            /* num_items() */
            return getCardinality();
        default:
            return -1;
    }
}

void process_message(struct message *msg) {
    struct message msg_local;   /* local message */
    mqd_t q_client;      /* client queue */

    /* thread copies message to local message */
    pthread_mutex_lock(&mutex_msg);
    memcpy(&msg_local, msg, sizeof(struct message));

    /* execute client request and prepare reply */
    printf("\n********************\n");
    printf("Received request from: %s\n", (char*)msg_local.q_name);
    printf("MESSAGE RECEIVED\n");
    printf("id_method: %d\n", msg_local.id_method);
    printf("key: %d\n", msg_local.key);
    printf("value1: %s\n", msg_local.value1);
    printf("value2: %f\n", msg_local.value2);

    /* Data structure access protected */
    int result = getResponse(&msg_local);

    /* wake up server */
    msg_not_copied = FALSE;

    pthread_cond_signal(&cond_msg);

    pthread_mutex_unlock(&mutex_msg);

    /* return result to client by sending it to queue */
    q_client = mq_open(msg_local.q_name, O_WRONLY);

    msg_local.id_method = result;

    printf("Code from server: %d\n", result);

    printf("MESSAGE SENT\n");
    printf("id_method: %d\n", msg_local.id_method);
    printf("key: %d\n", msg_local.key);
    printf("value1: %s\n", msg_local.value1);
    printf("value2: %f\n", msg_local.value2);

    if (q_client == -1)
        perror("Can't open client queue");
    else {
        mq_send(q_client, (const char*) &msg_local, sizeof(struct message), 0);
        printf("Msg sent\n");

        mq_close(q_client);
        printf("Queue closed\n");
        printf("********************\n");
    }
    pthread_exit(0);
}
