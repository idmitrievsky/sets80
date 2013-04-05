//
//  sets.c
//  Sets
//
//  Created by ivan on 02.04.13.
//  Copyright (c) 2013 dmitrievsky. All rights reserved.
//

#include <stdlib.h>
#include <string.h>
#include "sets.h"

ErrorCode CreateSet(Set **set)
{
    Set *s = NULL;
    
    s = malloc(sizeof(Set));
    MEM(s, errHandler);
    s->name = NULL;
    s->elems = NULL;
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
    free((*set)->name);
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
    return Length(A->elems);
}

int IsSubset(Set *A, Set *B)
{
    Node *elemA = NULL, *elemB = NULL;
    
    elemA = Select(A);
    elemB = Select(B);
    
    while (!IsLastIn(elemA, A) && !IsLastIn(elemB, B))
    {
        if (IsBefore(elemA, elemB))
        {
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
    return IsLastIn(elemA, A);
}

int IsMember(char *string, Set *A)
{
    return !!(Find(A->elems, string));
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

ErrorCode NameSetAfter(Set *C, Set *A, Set *B, char *del)
{
    char *name = NULL;
    ErrorCode errorCode = ERRORCODE_NO_ERROR;
    name = malloc(strlen(A->name) + strlen(B->name) + 2);
    MEM(name, errHandler);
    strcpy(name, A->name);
    strcat(name, del);
    strcat(name, B->name);
    if (C->name != NULL)
    {
        free(C->name);
    }
    CATCH_ERROR(NameSet(C, name), errHandler);
    free(name);
    
    return ERRORCODE_NO_ERROR;
    
    errHandler:
        return errorCode;
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
    
    CATCH_ERROR(NameSetAfter(*C, A, B, "U"), errHandler);
    
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
    
    CATCH_ERROR(NameSetAfter(*C, A, B, "X"), errHandler);
    
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
    
    CATCH_ERROR(NameSetAfter(*C, A, B, "+"), errHandler);
    
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
    
    CATCH_ERROR(NameSetAfter(*C, A, B, "\\"), errHandler);
    
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
