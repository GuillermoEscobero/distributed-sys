/****************************************************************************
 * client.c
 *    Example of use of library libkeys. This contains several tests to
 *    check the functionalities required.
 * Author: Guillermo Escobero
 ****************************************************************************/

#include <stdio.h>
#include "keys.h"

int main(void) {
    char res1[MAXSIZE];

	  /*strcpy(res1, "keloke\0");*/

    float res2 = 0.0;

    printf("********** TEST1: INIT ************\n");
    init();
    printf("Done\n");

    printf("\n********** TEST2: SET_VALUE ************\n");
    printf("Code expected: 0, got: %d\n", set_value(3, "testid3\0", 3.33));
    printf("Done\n");
    printf("********** TEST3: set key already used ************\n");
    printf("Code expected: -1, got: %d\n", set_value(3, "SSSSSS3\0", 321.33));

    printf("\n********** TEST4: GET_VALUE ************\n");
    printf("Code expected: 0, got: %d\n", get_value(3, res1, &res2));
    printf("Expected: testid3 (value1) from server: %s\n", res1);
    printf("Expected: 3.33 (value2) from server: %f\n", res2);

    strcpy(res1, "modifiedid3\0");
    res2 = 4.44;

    printf("\n********** TEST5: MODIFY_VALUE ************\n");
    printf("Code expected: 0, got: %d\n", modify_value(3, res1, &res2));

    printf("\n********** TEST6: check test 5 ************\n");
    printf("Code expected: 0, got: %d\n", get_value(3, res1, &res2));
    strcpy(res1, "clean\0");
    res2 = 0.00;
    printf("Expected: modifiedid3 (value1) from server: %s\n", res1);
    printf("Expected: 4.44 (value2) from server: %f\n", res2);

    printf("\n********** TEST7: NUM_ITEMS ************\n");
    printf("Add another triplet -> Code expected: 0, got: %d\n", set_value(56, "testid56\0", 57.75));
    printf("Add another triplet -> Code expected: 0, got: %d\n", set_value(372, "testid372\0", 372.43));

    printf("Items expected: 3, sent by server: %d\n", num_items());

    printf("\n********** TEST4: GET_VALUE ************\n");
    printf("Code expected: 0, got: %d\n", get_value(56, res1, &res2));
    printf("Expected: testid3 (value1) from server: %s\n", res1);
    printf("Expected: 3.33 (value2) from server: %f\n", res2);

    printf("\n********** TEST7: DELETE_KEY ************\n");
    printf("Code expected: 0, got: %d\n", delete_key(3));
    printf("Try to delete again: Code expected: -1, got: %d\n", delete_key(3));
    printf("Items expected: 2, sent by server: %d\n", num_items());


}
