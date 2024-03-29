/**************************************************************************
*
* Copyright (C) 2012 Steve Karg <skarg@users.sourceforge.net>
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************/
#ifndef KEYLIST_H
#define KEYLIST_H

#include "bacnet/bacnet_stack_exports.h"
#include "bacnet/basic/sys/platform.h"
#include "bacnet/basic/sys/key.h"

/* This is a key sorted linked list data library that */
/* uses a key or index to access the data. */
/* If the keys are duplicated, they can be added into the list like FIFO */

/* list data and datatype */
struct Keylist_Node {
    KEY key;    /* unique number that is sorted in the list */
    void *data; /* pointer to some data that is stored */
};

typedef struct Keylist {
    struct Keylist_Node **array;        /* array of nodes */
    int count;  /* number of nodes in this list - more efficient than loop */
    int size;   /* number of available nodes on this list - can grow or shrink */
} KEYLIST_TYPE;
typedef KEYLIST_TYPE *OS_Keylist;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* returns head of the list or NULL on failure. */
    BACNET_STACK_EXPORT
    OS_Keylist Keylist_Create(
        void);

/* delete specified list */
/* note: you should pop all the nodes off the list first. */
    BACNET_STACK_EXPORT
    void Keylist_Delete(
        OS_Keylist list);

/* inserts a node into its sorted position */
/* returns the index where it was added */
    BACNET_STACK_EXPORT
    int Keylist_Data_Add(
        OS_Keylist list,
        KEY key,
        void *data);

/* deletes a node specified by its key */
    BACNET_STACK_EXPORT
/* returns the data from the node */
    void *Keylist_Data_Delete(
        OS_Keylist list,
        KEY key);

/* deletes a node specified by its index */
    BACNET_STACK_EXPORT
/* returns the data from the node */
    void *Keylist_Data_Delete_By_Index(
        OS_Keylist list,
        int index);

/* returns the data from last node, and removes it from the list */
    BACNET_STACK_EXPORT
    void *Keylist_Data_Pop(
        OS_Keylist list);

/* returns the data from the node specified by key */
    BACNET_STACK_EXPORT
    void *Keylist_Data(
        OS_Keylist list,
        KEY key);

/* returns the index from the node specified by key */
    BACNET_STACK_EXPORT
    int Keylist_Index(
        OS_Keylist list,
        KEY key);

/* returns the data specified by key */
    BACNET_STACK_EXPORT
    void *Keylist_Data_Index(
        OS_Keylist list,
        int index);

/* returns the node key specified by the index */
    BACNET_STACK_DEPRECATED("Use Keylist_Index_Key() instead")
    BACNET_STACK_EXPORT
    KEY Keylist_Key(
        OS_Keylist list,
        int index);

/* returns the node key specified by the index */
    BACNET_STACK_EXPORT
    bool Keylist_Index_Key(
        OS_Keylist list, 
        int index, 
        KEY *pKey);

/* returns the next empty key from the list */
    BACNET_STACK_EXPORT
    KEY Keylist_Next_Empty_Key(
        OS_Keylist list,
        KEY key);

/* returns the number of items in the list */
    BACNET_STACK_EXPORT
    int Keylist_Count(
        OS_Keylist list);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
