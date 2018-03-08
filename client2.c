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

	printf("Num items: %d\n", num_items());

	printf("Code from getValue: %d\n", get_value(56, res1, &res2));

	strcpy(res1, "NOLOCREO");
	res2 = 999.999;
	printf("MODIFING 56: %s and %f\n", res1, res2);
	modify_value(56, res1, &res2);
	set_value(123, "GABOLO", 123.123123);
	
	printf("Code from getValue: %d\n", get_value(123, res1, &res2));
	printf("MODIFING 123: %s and %f\n", res1, res2);
}
