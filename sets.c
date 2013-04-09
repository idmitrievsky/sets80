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

ErrorCode CreateSet(Set **set) // creates set with inited list of elems
{
    Set *s = NULL;
    ErrorCode errorCode = ERRORCODE_NO_ERROR;
    
    s = malloc(sizeof(Set));
    MEM(s, errHandler);
    s->name = NULL;
    CATCH_ERROR(CreateList(&(s->elems)), errHandler);
    s->next = NULL;
    *set = s;
    
    return ERRORCODE_NO_ERROR;
    
    errHandler:
        return errorCode;
}

ErrorCode CreateSetWithName(Set **set, char* name) // creates set with a given name
{
    ErrorCode errorCode = ERRORCODE_NO_ERROR;
    
    CATCH_ERROR(CreateSet(set), errHandler);
    (*set)->name = malloc(strlen(name) + 1);
    MEM((*set)->name, errHandler);
    strcpy((*set)->name, name);
    
    return ERRORCODE_NO_ERROR;
    
    errHandler:
        return errorCode;
}

ErrorCode AddToSet(Set **A, char *string) // adds elem to set with respect to order
{
    Set *set = NULL;
    ErrorCode errorCode = ERRORCODE_NO_ERROR;
    
    if (strlen(string) > 80)
    {
        return ERRORCODE_TOO_LONG_STRING;
    }
    
    set = *A;
    
    CATCH_ERROR(AddNode(&(set->elems), string), errHandler);
    
    return ERRORCODE_NO_ERROR;
    
    errHandler:
        return errorCode;
}

ErrorCode AddAfterElement(Set **A, Node *to, char *string, Node **added) // adds elem after <to> node, order should be kept manually!
{
    Set *set = NULL;
    Node *newNode = NULL;
    ErrorCode errorCode = ERRORCODE_NO_ERROR;
    
    set = *A;
    
    if (!to)
    {
        to = set->elems->head;
    }
    newNode = malloc(sizeof(Node));
    MEM(newNode, errHandler);
    newNode->next = to->next;
    newNode->content = string;
    to->next = newNode;
    
    *added = newNode;
    
    return ERRORCODE_NO_ERROR;
    
errHandler:
    return errorCode;
}

ErrorCode DeleteFromSet(Set *A, char *string) // removes <string> from set
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

int IsLastIn(Node *node, Set *set) // checks if elem is last in set
{
    return IsTailOf(node, set->elems);
}

Node *Select(Set *set) // picks first elem of set
{
    return set->elems->head->next;
}

int Cardinality(Set *A) // calculates and prints to stdout # of set
{
    int card = 0;
    
    printf("%d\n", card = Length(A->elems));
    return card;
}

int IsSubset(Set *A, Set *B) // checks and prints if A is a subset of B
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

int IsMember(char *string, Set *A) // checks and prints if <string> is a member of A
{
    int ans = 0;
    ans = !!(Find(A->elems, string));
    printf("%s\n", ans ? "Yes" : "No");
    return ans;
}

ErrorCode NameSet(Set *set, char *name) // names existing set
{
    ErrorCode errorCode = ERRORCODE_NO_ERROR;
    
    set->name = malloc(strlen(name) + 1);
    MEM(set->name, errHandler);
    strcpy(set->name, name);
    
    return ERRORCODE_NO_ERROR;
    
    errHandler:
        return errorCode;
}

ErrorCode AddToFrom(Set **A, Node *to, Node *start, Set *from, Node **added) // adds all elems from <from> set starting with <start> to A set after <to>
{
    ErrorCode errorCode = ERRORCODE_NO_ERROR;
    Node *node = NULL, *lastAdded = NULL;
    
    node = start;
    lastAdded = to;
    
    while (!IsLastIn(node, from))
    {
        CATCH_ERROR(AddAfterElement(A, lastAdded, node->content, &lastAdded), errHandler);
        node = node->next;
    }
    
    return ERRORCODE_NO_ERROR;
    
    errHandler:
        return errorCode;
}

ErrorCode Union(Set *A, Set *B, Set **C) // creates a union of A and B
{
    Node *elemA = NULL, *elemB = NULL, *elemC = NULL, *lastAdded = NULL;
    
    ErrorCode errorCode;

    elemA = Select(A);
    elemB = Select(B);
    
    CATCH_ERROR(CreateSet(C), errHandler);
    
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
        CATCH_ERROR(AddAfterElement(C, lastAdded, elemC->content, &lastAdded), errHandler);
    }
    CATCH_ERROR(AddToFrom(C, lastAdded, elemA, A, &lastAdded), errHandler);
    CATCH_ERROR(AddToFrom(C, lastAdded, elemB, B, &lastAdded), errHandler);
    
    return ERRORCODE_NO_ERROR;
    
    errHandler:
        DeleteSet(C);
        return errorCode;
}

ErrorCode Intersection(Set *A, Set *B, Set **C) // creates a intersection of A and B
{
    Node *elemA = NULL, *elemB = NULL, *lastAdded = NULL;
    
    ErrorCode errorCode;
    
    elemA = Select(A);
    elemB = Select(B);
    
    CATCH_ERROR(CreateSet(C), errHandler);
    
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
            CATCH_ERROR(AddAfterElement(C, lastAdded, elemA->content, &lastAdded), errHandler);
            elemA = NextNode(elemA);
            elemB = NextNode(elemB);
        }
    }
    
    return ERRORCODE_NO_ERROR;
    
    errHandler:
        DeleteSet(C);
        return errorCode;
}

ErrorCode SymmDifference(Set *A, Set *B, Set **C) // creates a symm difference of A and B
{
    Node *elemA = NULL, *elemB = NULL, *lastAdded = NULL;
    
    ErrorCode errorCode;
    
    elemA = Select(A);
    elemB = Select(B);
    
    CATCH_ERROR(CreateSet(C), errHandler);
    
    while (!IsLastIn(elemA, A) && !IsLastIn(elemB, B))
    {
        if (IsBefore(elemA, elemB))
        {
            CATCH_ERROR(AddAfterElement(C, lastAdded, elemA->content, &lastAdded), errHandler);
            elemA = NextNode(elemA);
        }
        else if (IsBefore(elemB, elemA))
        {
            CATCH_ERROR(AddAfterElement(C, lastAdded, elemB->content, &lastAdded), errHandler);
            elemB = NextNode(elemB);
        }
        else
        {
            elemA = NextNode(elemA);
            elemB = NextNode(elemB);
        }
    }
    CATCH_ERROR(AddToFrom(C, lastAdded, elemA, A, &lastAdded), errHandler);
    CATCH_ERROR(AddToFrom(C, lastAdded, elemB, B, &lastAdded), errHandler);
    
    return ERRORCODE_NO_ERROR;
    
    errHandler:
        DeleteSet(C);
        return errorCode;
}

ErrorCode Substract(Set *A, Set *B, Set **C)
{
    Node *elemA = NULL, *elemB = NULL, *lastAdded = NULL;
    
    ErrorCode errorCode;
    
    elemA = Select(A);
    elemB = Select(B);
    
    CATCH_ERROR(CreateSet(C), errHandler);
    
    while (!IsLastIn(elemA, A) && !IsLastIn(elemB, B))
    {
        if (IsBefore(elemA, elemB))
        {
            CATCH_ERROR(AddAfterElement(C, lastAdded, elemA->content, &lastAdded), errHandler);
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
    CATCH_ERROR(AddToFrom(C, lastAdded, elemA, A, &lastAdded), errHandler);
    
    return ERRORCODE_NO_ERROR;
    
    errHandler:
        DeleteSet(C);
        return errorCode;
}

ErrorCode CreateSetList(SetList **setList) // creates a setlist
{
    ErrorCode errorCode = ERRORCODE_NO_ERROR;
    
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
    return errorCode;
}

Set *FindInSetList(SetList *setList, char *name) // returns set before found, or nothing
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


ErrorCode FindInSetListExact(SetList *setList, char *name, Set **found) // founds exactly needed set
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

ErrorCode AddEmptySet(SetList **_setList, char *name) // creates and adds named empty set
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

ErrorCode AddSet(SetList **_setList, Set *set) // adds set
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

ErrorCode AddSetCombination(SetList **_setList, ErrorCode (*Op) (Set *A, Set *B, Set **C), Set *A, Set *B, char *newName) // adds set combination
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

void ReleaseSetList(SetList **setList) // releases set list
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

void DeleteSetFrom(SetList *setList, char *name) // removes set from list
{
    Set *temp = NULL, *found = NULL;
    
    if ((temp = FindInSetList(setList, name)))
    {
        found = temp->next;
        temp->next = found->next;
        DeleteSet(&found);
    }
}

void PrintSetList(SetList *setList) // prints set list
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

