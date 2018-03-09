/****************************************************************************
 * client.c
 *    Example of use of library libkeys. This contains several tests to
 *    check the functionalities required.
 * Author: Guillermo Escobero
 ****************************************************************************/

#include <stdio.h>
#include <string.h>
#include "keys.h"

int main(void) {
    char res1[MAXSIZE] = {0};

    float res2 = 0.0;

    printf("********** TEST1: INIT ************\n");
    printf("Code expected: 0, got: %d\n", init());

    printf("\n********** TEST2: SET_VALUE ************\n");
    printf("Code expected: 0, got: %d\n", set_value(3, "testid3", 3.33));

    printf("\n********** TEST3: set key already used ************\n");
    printf("Code expected: -1, got: %d\n", set_value(3, "SSSSSS3", 321.33));

    printf("\n********** TEST4: GET_VALUE ************\n");
    printf("Code expected: 0, got: %d\n", get_value(3, res1, &res2));
    printf("Expected: testid3 (value1) from server: %s\n", res1);
    printf("Expected: 3.33 (value2) from server: %f\n", res2);

    printf("\n********** TEST4: GET_VALUE (no existing key) ************\n");
    printf("Code expected: -1, got: %d\n", get_value(89, res1, &res2));

    strcpy(res1, "modifiedid3");
    res2 = 4.44;

    printf("\n********** TEST5: MODIFY_VALUE ************\n");
    printf("Code expected: 0, got: %d\n", modify_value(3, res1, &res2));

    strcpy(res1, "clean\0");
    res2 = 0.00;

    printf("\n********** TEST6: check test 5 ************\n");
    printf("Code expected: 0, got: %d\n", get_value(3, res1, &res2));

    printf("Expected: modifiedid3 (value1) from server: %s\n", res1);
    printf("Expected: 4.44 (value2) from server: %f\n", res2);

    printf("\n********** TEST7: NUM_ITEMS ************\n");
    printf("Add another triplet -> Code expected: 0, got: %d\n", set_value(56, "testid56", 57.75));
    printf("Add another triplet -> Code expected: 0, got: %d\n", set_value(372, "testid372", 372.43));

    printf("Items expected: 3, sent by server: %d\n", num_items());


    printf("\n********** TEST8: DELETE_KEY ************\n");
    printf("Deleting key 3\n");
    printf("Code expected: 0, got: %d\n", delete_key(3));
    printf("Try to delete again key 3: Code expected: -1, got: %d\n", delete_key(3));
    printf("Items expected: 2, sent by server: %d\n", num_items());

    printf("\n********** TEST9: SET_VALUE deleted ************\n");
    printf("Code expected: 0, got: %d\n", set_value(3, "new3", 393.12));

    printf("\n********** TEST10: check test 9 ************\n");
    printf("Code expected: 0, got: %d\n", get_value(3, res1, &res2));
    printf("Expected: new3 (value1) from server: %s\n", res1);
    printf("Expected: 393.12 (value2) from server: %f\n", res2);

    printf("\n********** TEST11: INIT CURRENT STATE ************\n");
    printf("Code expected: 0, got: %d\n", init());
    printf("Getting key 56...\n");
    printf("Code expected: -1, got: %d\n", get_value(56, res1, &res2));
    printf("Items expected: 0, sent by server: %d\n", num_items());

    return 0;
}
