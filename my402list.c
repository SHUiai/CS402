/******************************************************************************/
/* Important CSCI 402 usage information:                                      */
/*                                                                            */
/* This fils is part of CSCI 402 programming assignments at USC.              */
/*         53616c7465645f5f2e8d450c0c5851acd538befe33744efca0f1c4f9fb5f       */
/*         3c8feabc561a99e53d4d21951738da923cd1c7bbd11b30a1afb11172f80b       */
/*         984b1acfbbf8fae6ea57e0583d2610a618379293cb1de8e1e9d07e6287e8       */
/*         de7e82f3d48866aa2009b599e92c852f7dbf7a6e573f1c7228ca34b9f368       */
/*         faaef0c0fcf294cb                                                   */
/* Please understand that you are NOT permitted to distribute or publically   */
/*         display a copy of this file (or ANY PART of it) for any reason.    */
/* If anyone (including your prospective employer) asks you to post the code, */
/*         you must inform them that you do NOT have permissions to do so.    */
/* You are also NOT permitted to remove or alter this comment block.          */
/* If this comment block is removed or altered in a submitted file, 20 points */
/*         will be deducted.                                                  */
/******************************************************************************/

/*
 * Author:      William Chia-Wei Cheng (bill.cheng@acm.org)
 *
 * @(#)$Id: listtest.c,v 1.1 2016/12/21 19:22:34 william Exp $
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

#include "cs402.h"

#include "my402list.h"

int  My402ListLength(My402List* l)
{
    return l->num_members;
}
int  My402ListEmpty(My402List* l)
{
    if (l->num_members == 0) {
        return TRUE;
    }else{
        return FALSE;
    }
}

int  My402ListAppend(My402List* l, void* newObj)
{
    My402ListElem* newElem =(My402ListElem*)malloc(sizeof(My402ListElem));
    if (newElem == NULL) {
        return FALSE;
    }
    newElem->obj = newObj;
    if (l->num_members == 0){
        l->anchor.next = newElem;
        l->anchor.prev = newElem;
        newElem->next = &(l->anchor);
        newElem->prev = &(l->anchor);
    }else{
        My402ListElem* lastElem = My402ListLast(l);
        lastElem->next = newElem;
        newElem->next = &(l->anchor);
        newElem->prev = lastElem;
        l->anchor.prev = newElem;
    }
    l->num_members++;
    return TRUE;

}
int  My402ListPrepend(My402List* l, void* newObj)
{
    My402ListElem* newElem =(My402ListElem*)malloc(sizeof(My402ListElem));
    if (newElem == NULL) {
        return FALSE;
    }
    newElem->obj = newObj;
    if (l->num_members == 0){
        l->anchor.next = newElem;
        l->anchor.prev = newElem;
        newElem->next = &(l->anchor);
        newElem->prev = &(l->anchor);
    }else{
        My402ListElem* firstElem = My402ListFirst(l);
        firstElem->prev = newElem;
        newElem->prev = &(l->anchor);
        newElem->next = firstElem;
        l->anchor.next = newElem;
    }
    l->num_members++;
    return TRUE;

}
void My402ListUnlink(My402List* l, My402ListElem* e) 
{
    e->next->prev = e->prev;
    e->prev->next = e->next;
    free(e);
    l->num_members--;
}
void My402ListUnlinkAll(My402List* l)
{
    My402ListElem* temp = l->anchor.next;
    while (temp!= &(l->anchor)){
        temp->next->prev = temp->prev;
        temp->prev->next = temp->next;
        My402ListElem* tempp = temp;
        temp = temp->next;
        free(tempp);
        l->num_members--;

    }
}
int  My402ListInsertAfter(My402List* l, void* newObj, My402ListElem* e)
{
    if (e == NULL){
        return My402ListAppend(l, newObj);
    }else{
        My402ListElem* newElem =(My402ListElem*)malloc(sizeof(My402ListElem));
        if (newElem == NULL) {
            return FALSE;
        }
        newElem->obj = newObj;
        l->num_members++;
        newElem->prev = e;
        newElem->next = e->next;
        e->next->prev = newElem;
        e->next = newElem;
        return TRUE;
    }
}
int  My402ListInsertBefore(My402List* l, void* newObj, My402ListElem* e)
{
    if (e == NULL){
        return My402ListPrepend(l, newObj);
    }else{
        My402ListElem* newElem =(My402ListElem*)malloc(sizeof(My402ListElem));
        if (newElem == NULL) {
            return FALSE;
        }
        newElem->obj = newObj;
        l->num_members++;
        newElem->prev = e->prev;
        newElem->next = e;
        e->prev->next = newElem;
        e->prev = newElem;
        return TRUE;
    }
}

My402ListElem *My402ListFirst(My402List* l)
{
    if (l-> num_members != 0){
        return l-> anchor.next;
    }else{
        return NULL;
    }
}



My402ListElem *My402ListLast(My402List* l)
{
    if (l-> num_members != 0){
        return l-> anchor.prev;
    }else{
        return NULL;
    }
}

My402ListElem *My402ListNext(My402List* l, My402ListElem* e)
{
    if (e->next != &(l->anchor)){
        return e->next;
    }else{
        return NULL;
    }
}
My402ListElem *My402ListPrev(My402List* l, My402ListElem* e)
{
    if (e->prev != &(l->anchor)){
        return e->prev;
    }else{
        return NULL;
    }

}

My402ListElem *My402ListFind(My402List* l, void* target)
{
    My402ListElem* temp = l->anchor.next;   
    while (temp != &(l->anchor)){
        if (temp->obj == target){
            return temp;
            break;
        }
        temp = temp->next;
    }
    return NULL;
}
int My402ListInit(My402List* l)
{
    
    l->anchor.next = &(l->anchor);
    l->anchor.prev = &(l->anchor);
    l->num_members = 0; 
    l->anchor.obj = NULL;
    return TRUE;
}
