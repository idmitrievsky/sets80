//
//  sets.c
//  Sets
//
//  Created by ivan on 02.04.13.
//  Copyright (c) 2013 dmitrievsky. All rights reserved.
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sets.h"

ErrorCode CreateSet(Set **set)
{
    Set *s = NULL;
    
    s = malloc(sizeof(Set));
    MEM(s, errHandler);
    s->name = NULL;
    CreateList(&(s->elems));
    s->next = NULL;
    *set = s;
    
    return ERRORCODE_NO_ERROR;
    
    errHandler:
        return ERRORCODE_MEMORY_ALLOCATION_ERROR;
}

ErrorCode CreateSetWithName(Set **set, char* name)
{
    ErrorCode errorCode = ERRORCODE_NO_ERROR;
    
    CATCH_ERROR(CreateSet(set), errHandler);
    (*set)->name = malloc(strlen(name) + 1);
    MEM((*set)->name, errHandler);
    strcpy((*set)->name, name);
    
    return ERRORCODE_NO_ERROR;
    
    errHandler:
        return ERRORCODE_MEMORY_ALLOCATION_ERROR;
}

ErrorCode AddToSet(Set **A, char *string)
{
    Set *set = NULL;
    ErrorCode errorCode = ERRORCODE_NO_ERROR;
    
    if (strlen(string) > 80)
    {
        return ERRORCODE_TOO_LONG_STRING;
    }
    
    set = *A;
    
    if (!set)
    {
        CATCH_ERROR(CreateSet(A), errHandler);
    }
    
    set = *A;
    
    CATCH_ERROR(AddNode(&(set->elems), string), errHandler);
    
    return ERRORCODE_NO_ERROR;
    
    errHandler:
        return errorCode;
}

ErrorCode DeleteFromSet(Set *A, char *string)
{
    if (strlen(string) > 80)
    {
        return ERRORCODE_TOO_LONG_STRING;
    }
    
    DeleteNode(A->elems, string);
    
    return ERRORCODE_NO_ERROR;
}

void DeleteSet(Set **set)
{
    if (*set == NULL)
    {
        return;
    }
    
    ReleaseList(&(*set)->elems);
    //free((*set)->name);
    free(*set);
    *set = NULL;
}

int IsLastIn(Node *node, Set *set)
{
    return IsTailOf(node, set->elems);
}

Node *Select(Set *set)
{
    return set->elems->head->next;
}

int Cardinality(Set *A)
{
    int card = 0;
    
    printf("%d\n", card = Length(A->elems));
    return card;
}

int IsSubset(Set *A, Set *B)
{
    int ans = 0;
    Node *elemA = NULL, *elemB = NULL;
    
    elemA = Select(A);
    elemB = Select(B);
    
    while (!IsLastIn(elemA, A) && !IsLastIn(elemB, B))
    {
        if (IsBefore(elemA, elemB))
        {
            printf("%s\n", "No");
            return 0;
        }
        else if (IsBefore(elemB, elemA))
        {
            elemB = NextNode(elemB);
        }
        else
        {
            elemA = NextNode(elemA);
            elemB = NextNode(elemB);
        }
    }
    
    ans = IsLastIn(elemA, A);
    printf("%s\n", ans ? "Yes" : "No");
    return ans;
}

int IsMember(char *string, Set *A)
{
    int ans = 0;
    ans = !!(Find(A->elems, string));
    printf("%s\n", ans ? "Yes" : "No");
    return ans;
}

ErrorCode NameSet(Set *set, char *name)
{
    set->name = malloc(strlen(name) + 1);
    MEM(set->name, errHandler);
    strcpy(set->name, name);
    
    return ERRORCODE_NO_ERROR;
    
    errHandler:
        return ERRORCODE_MEMORY_ALLOCATION_ERROR;
}

ErrorCode AddToFrom(Set **to, Node *start, Set *from)
{
    ErrorCode errorCode = ERRORCODE_NO_ERROR;
    
    if (*to == NULL)
    {
        CATCH_ERROR(CreateSet(to), errHandler);
    }
    CATCH_ERROR(AppendToFrom(&((*to)->elems), start, from->elems), errHandler);
    
    return ERRORCODE_NO_ERROR;
    
    errHandler:
        DeleteSet(to);
        return errorCode;
}

ErrorCode Union(Set *A, Set *B, Set **C)
{
    Node *elemA = NULL, *elemB = NULL, *elemC = NULL;
    
    ErrorCode errorCode;

    elemA = Select(A);
    elemB = Select(B);
    
    while (!IsLastIn(elemA, A) && !IsLastIn(elemB, B))
    {
        if (IsBefore(elemA, elemB))
        {
            elemC = elemA;
            elemA = NextNode(elemA);
        }
        else if (IsBefore(elemB, elemA))
        {
            elemC = elemB;
            elemB = NextNode(elemB);
        }
        else
        {
            elemC = elemA;
            elemA = NextNode(elemA);
            elemB = NextNode(elemB);
        }
        CATCH_ERROR(AddToSet(C, elemC->content), errHandler);
    }
    CATCH_ERROR(AddToFrom(C, elemA, A), errHandler);
    CATCH_ERROR(AddToFrom(C, elemB, B), errHandler);
    
    return ERRORCODE_NO_ERROR;
    
    errHandler:
        DeleteSet(C);
        return errorCode;
}

ErrorCode Intersection(Set *A, Set *B, Set **C)
{
    Node *elemA = NULL, *elemB = NULL;
    
    ErrorCode errorCode;
    
    elemA = Select(A);
    elemB = Select(B);
    
    CreateSet(C);
    while (!IsLastIn(elemA, A) && !IsLastIn(elemB, B))
    {
        if (IsBefore(elemA, elemB))
        {
            elemA = NextNode(elemA);
        }
        else if (IsBefore(elemB, elemA))
        {
            elemB = NextNode(elemB);
        }
        else
        {
            CATCH_ERROR(AddToSet(C, elemA->content), errHandler);
            elemA = NextNode(elemA);
            elemB = NextNode(elemB);
        }
    }
    
    return ERRORCODE_NO_ERROR;
    
    errHandler:
        DeleteSet(C);
        return errorCode;
}

ErrorCode SymmDifference(Set *A, Set *B, Set **C)
{
    Node *elemA = NULL, *elemB = NULL;
    
    ErrorCode errorCode;
    
    elemA = Select(A);
    elemB = Select(B);
    
    while (!IsLastIn(elemA, A) && !IsLastIn(elemB, B))
    {
        if (IsBefore(elemA, elemB))
        {
            CATCH_ERROR(AddToSet(C, elemA->content), errHandler);
            elemA = NextNode(elemA);
        }
        else if (IsBefore(elemB, elemA))
        {
            CATCH_ERROR(AddToSet(C, elemB->content), errHandler);
            elemB = NextNode(elemB);
        }
        else
        {
            elemA = NextNode(elemA);
            elemB = NextNode(elemB);
        }
    }
    CATCH_ERROR(AddToFrom(C, elemA, A), errHandler);
    CATCH_ERROR(AddToFrom(C, elemB, B), errHandler);
    
    return ERRORCODE_NO_ERROR;
    
    errHandler:
        DeleteSet(C);
        return errorCode;
}

ErrorCode Substract(Set *A, Set *B, Set **C)
{
    Node *elemA = NULL, *elemB = NULL;
    
    ErrorCode errorCode;
    
    elemA = Select(A);
    elemB = Select(B);
    
    while (!IsLastIn(elemA, A) && !IsLastIn(elemB, B))
    {
        if (IsBefore(elemA, elemB))
        {
            CATCH_ERROR(AddToSet(C, elemA->content), errHandler);
            elemA = NextNode(elemA);
        }
        else if (IsBefore(elemB, elemA))
        {
            elemB = NextNode(elemB);
        }
        else
        {
            elemA = NextNode(elemA);
            elemB = NextNode(elemB);
        }
    }
    CATCH_ERROR(AddToFrom(C, elemA, A), errHandler);
    
    return ERRORCODE_NO_ERROR;
    
    errHandler:
        DeleteSet(C);
        return errorCode;
}

ErrorCode CreateSetList(SetList **setList)
{
    SetList *sl = NULL;
    Set *h = NULL, *t = NULL;
    
    sl = malloc(sizeof(SetList));
    MEM(sl, errHandler);
    sl->head = NULL;
    sl->tail = NULL;
    *setList = sl;
    
    h = malloc(sizeof(Set));
    MEM(h, errHandler);
    h->elems = NULL;
    h->next = NULL;
    (*setList)->head = h;
    
    t = malloc(sizeof(Set));
    MEM(t, errHandler);
    t->elems = NULL;
    t->next = NULL;
    (*setList)->tail = t;
    
    h->next = t;
    
    return ERRORCODE_NO_ERROR;
    
errHandler:
    if (h != NULL)
    {
        free(*setList);
        free(h);
    }
    else
    {
        free(*setList);
    }
    return ERRORCODE_MEMORY_ALLOCATION_ERROR;
}

Set *FindInSetList(SetList *setList, char *name)
{
    Set *tail = NULL, *temp1 = NULL, *temp2 = NULL;
    
    if (setList == NULL)
    {
        return NULL;
    }
    
    tail = setList->tail;
    temp1 = setList->head;
    temp2 = temp1->next;
    
    tail->name = name;
    
    while (strcmp(temp2->name, name))
    {
        temp1 = temp2;
        temp2 = temp2->next;
    }
    if (temp2 == tail)
    {
        return NULL;
    }
    else
    {
        return temp1;
    }
}


ErrorCode FindInSetListExact(SetList *setList, char *name, Set **found)
{
    Set *tail = NULL, *temp1 = NULL, *temp2 = NULL;
    
    if (setList == NULL)
    {
        return ERRORCODE_NO_SUCH_SET;
    }
    
    tail = setList->tail;
    temp1 = setList->head;
    temp2 = temp1->next;
    
    tail->name = name;
    
    while (strcmp(temp2->name, name))
    {
        temp1 = temp2;
        temp2 = temp2->next;
    }
    if (temp2 == tail)
    {
        *found = NULL;
        return ERRORCODE_NO_SUCH_SET;
    }
    else
    {
        *found = temp2;
        return ERRORCODE_NO_ERROR;
    }
}

ErrorCode AddEmptySet(SetList **_setList, char *name)
{
    SetList *setList = NULL;
    Set *tail = NULL, *temp1 = NULL, *temp2 = NULL;
    Set *new = NULL;
    ErrorCode errorCode = ERRORCODE_NO_ERROR;
    
    setList = *_setList;
    
    if (!setList)
    {
        CATCH_ERROR(CreateSetList(_setList), errHandler);
    }
    
    setList = *_setList;
    
    tail = setList->tail;
    temp1  = setList->head;
    temp2 = temp1->next;
    
    if (FindInSetList(setList, name))
    {
        return ERRORCODE_ALREADY_EXISTS;
    }
    
    tail->name = NULL;
    
    while (temp2 != setList->tail)
    {
        temp1 = temp2;
        temp2 = temp2->next;
    }
    
    CATCH_ERROR(CreateSetWithName(&new, name), errHandler);
    new->next = temp2;
    temp1->next = new;
    
    return ERRORCODE_NO_ERROR;
    
errHandler:
    return errorCode;
}

ErrorCode AddSet(SetList **_setList, Set *set)
{
    SetList *setList = NULL;
    Set *tail = NULL, *temp1 = NULL, *temp2 = NULL;
    ErrorCode errorCode = ERRORCODE_NO_ERROR;
    
    setList = *_setList;
    
    if (!setList)
    {
        CATCH_ERROR(CreateSetList(_setList), errHandler);
    }
    
    setList = *_setList;
    
    tail = setList->tail;
    temp1  = setList->head;
    temp2 = temp1->next;
    
    if (FindInSetList(setList, set->name))
    {
        return ERRORCODE_ALREADY_EXISTS;
    }
    
    tail->name = NULL;
    
    while (temp2 != setList->tail)
    {
        temp1 = temp2;
        temp2 = temp2->next;
    }
    
    
    set->next = temp2;
    temp1->next = set;
    
    return ERRORCODE_NO_ERROR;
    
errHandler:
    return errorCode;
}

ErrorCode AddSetCombination(SetList **_setList, ErrorCode (*Op) (Set *A, Set *B, Set **C), Set *A, Set *B, char *newName)
{
    Set *C = NULL;
    ErrorCode errorCode = ERRORCODE_NO_ERROR;

    CATCH_ERROR(Op(A, B, &C), errHandler);
    CATCH_ERROR(NameSet(C, newName), errHandler);
    CATCH_ERROR(AddSet(_setList, C), errHandler);
    
    return ERRORCODE_NO_ERROR;
    
    errHandler:
        if (C != NULL)
        {
            DeleteSet(&C);
        }
        return errorCode;
}

void ReleaseSetList(SetList **setList)
{
    Set *temp = NULL, *tofree = NULL, *tail = NULL;
    
    if (*setList == NULL)
    {
        return;
    }
    
    temp = (*setList)->head->next;
    tail = (*setList)->tail;
    
    while (temp != tail)
    {
        tofree = temp;
        temp = temp->next;
        DeleteSet(&tofree);
    }
    DeleteSet(&tail);
    DeleteSet(&((*setList)->head));
    free(*setList);
    *setList = NULL;
}

void DeleteSetFrom(SetList *setList, char *name)
{
    Set *temp = NULL, *found = NULL;
    
    if ((temp = FindInSetList(setList, name)))
    {
        found = temp->next;
        temp->next = found->next;
        DeleteSet(&found);
    }
}

void PrintSetList(SetList *setList)
{
    Set *temp = NULL, *tail = NULL;
    
    if (!setList)
    {
        printf("No sets yet\n");
        return;
    }
    
    tail = setList->tail;
    
    temp = setList->head->next;
    
    if (temp == tail)
    {
        printf("No sets yet\n");
        return;
    }
    
    while (temp != tail)
    {
        printf("<%s>\n", temp->name);
        temp = temp->next;
    }
}

