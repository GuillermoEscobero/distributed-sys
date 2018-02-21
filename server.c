#include <stdio.h>
#include "avltree.h"
#include <stdlib.h>
#include <string.h>

#include <mqueue.h>
#include <pthread.h>
#include <stdio.h>

#define TRUE 1
#define FALSE 0
#define MAXSIZE 256

struct request {
    char q_name[MAXSIZE];
    int id_method;
    int key;
    char* value1;
    float value2;
};

void process_message(struct request *msg);

pthread_mutex_t mutex_msg;
int msg_not_copied = TRUE;
pthread_cond_t cond_msg;

/* Declaration of data structure */
Tree tree;

/* Number of triples in the data structure */
int counter;

typedef struct NodeData {
    int key;
    char value1[256];
    float value2;
} *NodeData;

int comp(void *a1, void *a2) {
    NodeData nd1 = (NodeData) a1;
    NodeData nd2 = (NodeData) a2;

    if(nd1->key < nd2->key) {
        return -1;
    } else if(nd1->key > nd2->key) {
        return +1;
    } else {
        return 0;
    }
}

void print(void *a) {
    NodeData nd = (NodeData) a;

    printf("key: %d value1: %c value2: %f\n", nd->key, nd->value1, nd->value2);
}

int init() {
    tree = Tree_New(comp, print);
    if(tree == NULL)
        return -1;

    return 0;

}

int set_value(int key, char *value1, float value2) {
    Node node;
    NodeData nd1 = (NodeData)malloc(sizeof(NodeData));
    nd1 -> key = key;
    strcpy(nd1->value1, value1);
    /* nd1 -> value1 = value1; */
    nd1 -> value2 = value2;

    if((node = Tree_SearchNode(tree, nd1)) == NULL) {
        Tree_Insert(tree, nd1);
        counter++;
        return 0;
    }

    return -1;
}

int get_value(int key, char *value1, float *value2) {
    Node node;
    NodeData nd1 = (NodeData)malloc(sizeof(NodeData));
    nd1 -> key = key;

    if((node = Tree_SearchNode(tree, nd1)) != NULL) {
        nd1 = Node_GetData(node);
        value1 = (nd1->value1);
        value2 = &(nd1->value2);
        return 0;
    } else {
        return -1;
    }
}

int modify_value(int key, char *value1, float *value2) {
    Node node;
    NodeData nd1 = (NodeData)malloc(sizeof(NodeData));
    nd1 -> key = key;

    if((node = Tree_SearchNode(tree, nd1)) != NULL) {
        nd1 = Node_GetData(node);
        strcpy(nd1->value1, value1);
        //nd1->value1 = value1;
        nd1->value2 = *value2;
        return 0;
    } else {
        return -1;
    }
}

int delete_key(int key) {
    Node node;
    NodeData nd1 = (NodeData)malloc(sizeof(NodeData));
    nd1->key = key;

    if((node = Tree_SearchNode(tree, nd1)) != NULL) {
        Tree_DeleteNode(tree, node);
        counter--;
        return 0;
    }

    return -1;
}

int num_items() {
    return counter;
}

int main(int argc, char* argv[]) {
    mqd_t q_server;         /* server queue */
    struct request msg;     /* message to receive */
    struct mq_attr q_attr;  /* queue attributes */
    pthread_attr_t t_attr;  /* thread attributes */

    q_attr.mq_maxmsg = 20;
    q_attr.mq_msgsize = sizeof(struct request);

    q_server = mq_open("SERVER", O_CREAT|O_RDONLY, 0700, &q_attr);
    if (q_server == -1) {
        perror("Can't create server queue");
        return 1;
    }

    pthread_mutex_init(&mutex_msg, NULL);
    pthread_cond_init(&cond_msg, NULL);
    pthread_attr_init(&t_attr);

    /* thread attributes */
    pthread_attr_setdetachstate(&t_attr, PTHREAD_CREATE_DETACHED);
    
    pthread_t thr;

    while(TRUE) {
        mq_receive(q_server,(char*) &msg, sizeof(struct request), 0);

        pthread_create(&thr, &t_attr, (void*)process_message, &msg);

        thr++;

        /* wait for thread to copy message */
        pthread_mutex_lock(&mutex_msg);
        while (msg_not_copied) {
            pthread_cond_wait(&cond_msg, &mutex_msg);
        }
        msg_not_copied = TRUE;
        pthread_mutex_unlock(&mutex_msg);
    }
}

void process_message(struct request *msg) {
    struct request msg_local;   /* local message */
    mqd_t q_client;      /* client queue */

    /* thread copies message to local message */
    pthread_mutex_lock(&mutex_msg);
    memcpy((char *) &msg_local, (char *) &msg, sizeof(struct request));

    /* wake up server */
    msg_not_copied = FALSE;

    pthread_cond_signal(&cond_msg);

    pthread_mutex_unlock(&mutex_msg);

    /* execute client request and prepare reply */
    result = msg_local.a + msg_local.b;

    /* return result to client by sending it to queue */
    q_client = mq_open(msg_local.name, O_WRONLY);

    getResult(msg_local.id_method, msg_local.key, msg_local.value1, msg_local.value2);

    if (q_client == -1)
        perror("Can't open client queue");
    else {
        mq_send(q_client, (char *) &result, sizeof(int), 0);
        mq_close(q_client);
    }
    pthread_exit(0);
}
