#define MAXSIZE 256

struct request {
    char q_name[MAXSIZE];
    int id_method;
    int key;
    char* value1;
    float* value2;
};
