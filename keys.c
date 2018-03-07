/****************************************************************************
 * keys.c
 *    Implementation of library libkeys. This contains methods used by the
 *    client to communicate with the server.
 * Author: Guillermo Escobero
 ****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <mqueue.h>

#include "keys.h"
#include "message.h"

int init() {
    /* Structure to save the server response */
    struct message msg_local;
    /* Only is needed the id_method, as initialization does not use any data */
    return start(0, 0, "", 0, &msg_local);
}

int set_value(int key, char *value1, float value2) {
    /* Structure to save the server response */
    struct message msg_local;
    return start(1, key, value1, value2, &msg_local);
}

int get_value(int key, char *value1, float *value2) {
    /* Structure to save the server response */
    struct message msg_local;

    /* Checks if server operation is OK and copies received data into memory
     * addresses provided */
    if(start(2, key, value1, *value2, &msg_local) == 0) {
        strcpy(value1, msg_local.value1);
        *value2 = msg_local.value2;
        return 0;
    }
    return -1;
}

int modify_value(int key, char *value1, float *value2) {
    /* Structure to save the server response */
    struct message msg_local;
    return start(3, key, value1, *value2, &msg_local);
}

int delete_key(int key) {
    /* Structure to save the server response */
    struct message msg_local;
    /* Only is needed the id_method and key, as no more data is needed */
    return start(4, key, "", 0, &msg_local);
}

int num_items() {
    /* Structure to save the server response */
    struct message msg_local;
    /* Only is needed the id_method, as no more data is needed */
    return start(5, 0, "", 0, &msg_local);
}

/* Establishes connection with server process and sends the request */
int start(int id_method, int key, char *value1, float value2, struct message *dest_ptr) {
    /* Check that value of char *value1 fits in the message field max size */
    if(strlen(value1) > MAXSIZE) {
      printf("Error: value1 too long. Maximum size is %d\n", MAXSIZE);
      return -1;
    }

    /* Queues declaration */
    mqd_t q_server;
    mqd_t q_client;

    /* Request struct declaration */
    struct message req;

    /* Attributes for client queue */
    struct mq_attr attr;

    /* One message, as the queue will be created and deleted for every request */
    attr.mq_maxmsg = 1;
    attr.mq_msgsize = sizeof(struct message);

    /* Open client and server queues */
    q_client = mq_open("/CLIENT_ONE", O_CREAT|O_RDWR, 0660, &attr);
    q_server = mq_open("/SERVER_01", O_WRONLY);

    /* Fill the request to be sent */
    /* ID of the client. This will tell the server where to send the response */
    strcpy(req.q_name, "/CLIENT_ONE");
    req.id_method = id_method;
    req.key = key;
    memset(req.value1, '/0', sizeof(req.value1));
    strcpy(req.value1, value1);
    req.value2 = value2;

    /* Verbose for debugging */
    printf("Request sent:\n");
    printf("id_method: %d\n", req.id_method);
    printf("key: %d\n", req.key);
    printf("value1: %s\n", req.value1);
    printf("value2: %f\n\n", req.value2);

    /* Send the request to server */
    printf("Sending message... ");
    mq_send(q_server, (char*) &req, sizeof(struct message), 0);
    printf("Done.\n");

    /* Receive the response message of the server, and save it on dest_ptr */
    printf("Waiting for response... ");
    mq_receive(q_client, (char*) dest_ptr, sizeof(struct request), 0);
    printf("Received.\n");

    /* Close the queues */
    mq_close(q_server);
    mq_close(q_client);

    /* Unlink client queue */
    mq_unlink("/CLIENT_ONE");

    return dest_ptr->id_method;
}
