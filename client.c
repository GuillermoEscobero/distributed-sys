#include <stdio.h>
#include "keys.h"

int main(void) {
    char res1[MAXSIZE];

	strcpy(res1, "keloke\0");

    float res2 = 0.0;

    printf("Executing init()...\n"); 
    init();
    printf("Done\n");

    printf("Executing set_value()...\n"); 
    set_value(3, "testid3\0", 3.33);
    printf("Done\n");
    set_value(3, "SSSSSS3\0", 321.33);

    printf("Executing get_value()...\n"); 
    int result = get_value(3, res1, &res2);
    printf("Done, finish code: %d\n", result);

    printf("\n\nRECEIVED v1? %s\n", res1);
    printf("RECEIVED v2? %f\n\n", res2);

    strcpy(res1,"modifiedid3\0");
    res2 = 4.44;

    modify_value(3, res1, &res2);

    get_value(3, res1, &res2);

    printf("%s\n", res1);
    printf("%f\n", res2);

    printf("Items: %d\n", num_items());

    delete_key(3);

    printf("Items: %d\n", num_items());
}
