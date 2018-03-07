/****************************************************************************
 * keys.h
 *    Declaration of library libkeys. This contains the methods that the
 *    client can use.
 * Author: Guillermo Escobero
 ****************************************************************************/

#ifndef KEYS_H_
#define KEYS_H_

#define MAXSIZE 256

/* Deletes all the triplets stored on the server. The function returns 0 on
 * success and -1 on error. */
int init();

/* This service inserts the element <key, value1, value2>. The service returns 0
 * if it was inserted successfully and -1 in case of error. */
int set_value(int key, char *value1, float value2);

/* This service allows you to obtain the values associated with the key. The
 * values are returned in value1 and value2. The function returns 0 on success
 * and -1 on error, for example, if there is no element with that key. */
int get_value(int key, char *value1, float *value2);

/* This service allows modifying the values associated with a key. The function
 * returns 0 on success and -1 on error, for example, if there is no element
 * with that key. */
int modify_value(int key, char *value1, float *value2);

/* This service allows you to delete the element whose key is key. The function
 * returns 0 on success and -1 on error. In case the key does not exist, -1 is
 * also returned. */
int delete_key(int key);

/* This service returns the number of items stored on the server. The call
 * returns -1 in case of error. */
int num_items();

#endif /* KEYS_H_ */
