#include "request.h"
#include <mqueue.h>
#include <string.h>

int start(int id_method, int key, char *value1, float *value2);

int init();

int set_value(int key, char *value1, float value2);

int get_value(int key, char *value1, float *value2);

int modify_value(int key, char *value1, float *value2);

int delete_key(int key);

int num_items();
