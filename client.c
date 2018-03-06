#include <stdio.h>
#include "keys.h"

int main(void) {
    char * res1 = "keloke\0";

    float res2 = 0.0;

    printf("Executing init()...\n"); 
    init();
    printf("Done\n");

    printf("Executing set_value()...\n"); 
    set_value(3, "testid3\0", 3.33);
    printf("Done\n");

    printf("Executing get_value()...\n"); 
    get_value(3, res1, &res2);
    printf("Done\n");

    printf("RECEIVED v1? %s\n", res1);
    printf("RECEIVED v2? %f\n", res2);

    res1 = "modifiedid3";
    res2 = 4.44;

    modify_value(3, res1, &res2);

    get_value(3, res1, &res2);

    printf("%s\n", res1);
    printf("%f\n", res2);

    printf("Items: %d\n", num_items());

    delete_key(3);

    printf("Items: %d\n", num_items());
}
