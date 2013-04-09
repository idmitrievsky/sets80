//
//  lists.c
//  Sets
//
//  Created by ivan on 02.04.13.
//  Copyright (c) 2013 dmitrievsky. All rights reserved.
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h> // debug
#include "errors.h"
#include "lists.h"

ErrorCode CreateList(List **list) // creates a new list with head and tail
{
    List *l = NULL;
    Node *h = NULL, *t = NULL;
    ErrorCode errorCode = ERRORCODE_NO_ERROR;
    
    l = malloc(sizeof(List));
    MEM(l, errHandler);
    l->head = NULL;
    l->tail = NULL;
    *list = l;
    
    h = malloc(sizeof(Node));
    MEM(h, errHandler);
    h->content = NULL;
    h->next = NULL;
    (*list)->head = h;
    
    t = malloc(sizeof(Node));
    MEM(t, errHandler);
    t->content = NULL;
    t->next = NULL;
    (*list)->tail = t;
    
    h->next = t;
    
    return ERRORCODE_NO_ERROR;
    
    errHandler:
        if (h != NULL)
        {
            free(*list);
            free(h);
        }
        else
        {
            free(*list);
        }
        return errorCode;
}

ErrorCode AddNode(List **_list, char *content) // adds <content> to list IN ORDER, if it's already there — does nothing without error
{
    List *list = NULL;
    Node *tail = NULL, *temp1 = NULL, *temp2 = NULL;
    Node *new = NULL;
    ErrorCode errorCode = ERRORCODE_NO_ERROR;
    
    list = *_list;
    
    if (!list)
    {
        CATCH_ERROR(CreateList(_list), errHandler);
    }
    
    list = *_list;
    
    tail = list->tail;
    temp1  = list->head;
    temp2 = temp1->next;
    
    if (Find(list, content))
    {
        return ERRORCODE_NO_ERROR;
    }
    
    tail->content = content;
    
    while (strcmp(temp2->content, content) < 0)
    {
        temp1 = temp2;
        temp2 = temp2->next;
    }
    
    new = malloc(sizeof(Node));
    MEM(new, errHandler);
    
    new->content = content;
    new->next = temp2;
    temp1->next = new;
    
    return ERRORCODE_NO_ERROR;
    
errHandler:
    return errorCode;
}


ErrorCode AddNodeToEndForced(List **_list, char *content) // adds <content> to list's end (can create a duplicate)
{
    List *list = NULL;
    Node *tail = NULL, *temp1 = NULL, *temp2 = NULL;
    Node *new = NULL;
    ErrorCode errorCode = ERRORCODE_NO_ERROR;
    
    list = *_list;
    
    if (!list)
    {
        CATCH_ERROR(CreateList(_list), errHandler);
    }
    
    list = *_list;
    
    tail = list->tail;
    temp1  = list->head;
    temp2 = temp1->next;
    
    tail->content = content;
    
    while (temp2 != tail)
    {
        temp1 = temp2;
        temp2 = temp2->next;
    }
    
    new = malloc(sizeof(Node));
    MEM(new, errHandler);
    
    new->content = content;
    new->next = temp2;
    temp1->next = new;
    
    return ERRORCODE_NO_ERROR;
    
errHandler:
    return errorCode;
}

int Length(List *list) // calculates length
{
    int i = 0;
    Node *temp = NULL;
    
    if (!list)
    {
        return 0;
    }
    
    temp = list->head;
    
    while (temp)
    {
        i++;
        temp = temp->next;
    }
    
    return i - 2;
}

Node *Find(List *list, char *content) // returns node before found if found, otherwise — NULL
{
    Node *tail = NULL, *temp1 = NULL, *temp2 = NULL;
    
    if (list == NULL)
    {
        return NULL;
    }
    
    tail = list->tail;
    temp1 = list->head;
    temp2 = temp1->next;
    
    tail->content = content;
    
    while (strcmp(temp2->content, content))
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

void DeleteNode(List *list, char *content) // removes node if found, otherwise does nothing
{
    Node *temp = NULL, *found = NULL;
    
    if ((temp = Find(list, content)))
    {
        found = temp->next;
        temp->next = found->next;
        //free(found->content);
        free(found);
    }
}

void PrintList(List *list) // prints list to stdout
{
    Node *temp = NULL, *tail = NULL;
    
    if (!list)
    {
        printf("No elements yet\n");
        return;
    }
    
    tail = list->tail;
    
    temp = list->head->next;
    
    if (temp == tail)
    {
        printf("No elements yet\n");
        return;
    }
    
    while (temp != tail)
    {
        printf("<%s>\n", temp->content);
        temp = temp->next;
    }
}

void ReleaseList(List **list) // removes list
{
    Node *temp = NULL, *tofree = NULL, *tail = NULL;
    
    if (*list == NULL)
    {
        return;
    }
    
    temp = (*list)->head->next;
    tail = (*list)->tail;
    
    while (temp != tail)
    {
        tofree = temp;
        temp = temp->next;
        //free(tofree->content);
        free(tofree);
    }
    free(tail);
    free((*list)->head);
    free(*list);
    *list = NULL;
}

int IsBefore(Node *node1, Node *node2) // checking if node1 is before node2 in ORDERED list
{
    return (strcmp(node1->content, node2->content) < 0);
}

Node *NextNode(Node *node) // return next node
{
    return node->next;
}

int IsTailOf(Node *node, List *list) // checking if node is a tail of given list
{
    return (node == list->tail);
}

//ErrorCode AppendToFrom(List **to, Node *start, List *from)
//{
//    Node *node = NULL;
//    ErrorCode errorCode = ERRORCODE_NO_ERROR;
//    
//    node = start;
//    
//    while (!IsTailOf(node, from))
//    {
//        CATCH_ERROR(AddNode(to, node->content), errHandler);
//        node = NextNode(node);
//    }
//    
//    return ERRORCODE_NO_ERROR;
//    
//errHandler:
//    return errorCode;
//}

ErrorCode AppendToFromForced(List **to, Node *start, List *from)
{
    Node *node = NULL;
    ErrorCode errorCode = ERRORCODE_NO_ERROR;
    
    node = start;
    
    while (!IsTailOf(node, from))
    {
        CATCH_ERROR(AddNodeToEndForced(to, node->content), errHandler);
        node = NextNode(node);
    }
    
    return ERRORCODE_NO_ERROR;
    
errHandler:
    return errorCode;
}






