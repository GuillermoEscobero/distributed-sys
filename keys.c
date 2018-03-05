#include "keys.h"
#include <stdio.h>

int start(int id_method, int key, char *value1, float value2, struct request *dest_ptr) {
    mqd_t q_server;
    mqd_t q_client;

    struct request req;
    struct mq_attr attr;

    attr.mq_maxmsg = 1;
    attr.mq_msgsize = sizeof(struct request);
    
    q_client = mq_open("/CLIENT_ONE", O_CREAT|O_RDONLY, 0700, &attr);

    q_server = mq_open("/test", O_WRONLY);

    strcpy(req.q_name, "/CLIENT_ONE");
    req.id_method = id_method;
    req.key = key;

    strcpy(req.value1, value1);
    req.value2 = value2;

    printf("%d\n", req.id_method);
    printf("%d\n", req.key);
    printf("%s\n", req.value1);
    printf("%f\n", req.value2);
    
    mq_send(q_server, (char*) &req, sizeof(struct request), 0);
    mq_receive(q_client, (char*) dest_ptr, sizeof(struct request), 0);

    mq_close(q_server);
    mq_close(q_client);
    mq_unlink("/CLIENT_ONE");

    return 0;
}

int init() {
    struct request msg_local;
    printf("kkk");
    return start(0, 0, "", 0, &msg_local);
}

int set_value(int key, char *value1, float value2) {
    struct request msg_local;
    return start(1, key, value1, value2, &msg_local);
}

int get_value(int key, char *value1, float *value2) {
    struct request msg_local;
    
    if(start(2, key, value1, *value2, &msg_local) == 0) {
        strcpy(value1, msg_local.value1);
        *value2 = msg_local.value2;
        return 0;
    }

    return -1;
}

int modify_value(int key, char *value1, float *value2) {
    struct request msg_local;
    return start(3, key, value1, *value2, &msg_local);
}

int delete_key(int key) {
    struct request msg_local;
    return start(4, key, "", 0, &msg_local);
}

int num_items() {
    struct request msg_local;
    return start(5, 0, "", 0, &msg_local);
}
