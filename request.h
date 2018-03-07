/****************************************************************************
 * request.h
 *    Definition of structure of the messages sent between queues.
 * Author: Guillermo Escobero
 ****************************************************************************/

#ifndef MESSAGE_H_
#define MESSAGE_H_

#define MAXSIZE 256

struct message {
    char q_name[MAXSIZE];
    int id_method;
    int key;
    char value1[MAXSIZE];
    float value2;
};

#endif /* MESSAGE_H */
