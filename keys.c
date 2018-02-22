#include "keys.h"

int start(int id_method, int key, char *value1, float *value2) {
    mqd_t q_server;
    mqd_t q_client;

    struct request req;
    int res;
    struct mq_attr attr;

    attr.mq_maxmsg = 1;
    attr.mq_msgsize = sizeof(int);
    
    q_client = mq_open("/CLIENT_ONE", O_CREAT|O_RDONLY, 0700, &attr);

    q_server = mq_open("/SERVER", O_WRONLY);

    strcpy(req.q_name, "/CLIENT_ONE");
    req.id_method = id_method;
    req.key = key;
    req.value1 = value1;
    req.value2 = value2;

    mq_send(q_server, (char*) &req, sizeof(struct request), 0);
    mq_receive(q_client, (char*) &res, sizeof(int), 0);

    mq_close(q_server);
    mq_close(q_client);
    mq_unlink("/CLIENT_ONE");

    return 0;
}

int init() {
    return start(0, 0, NULL, NULL);
}

int set_value(int key, char *value1, float value2) {
    return start(1, key, value1, &value2);
}

int get_value(int key, char *value1, float *value2) {
    return start(2, key, value1, value2);
}

int modify_value(int key, char *value1, float *value2) {
    return start(3, key, value1, value2);
}

int delete_key(int key) {
    return start(4, key, NULL, NULL);
}

int num_items() {
    return start(5, 0, NULL, NULL);
}
