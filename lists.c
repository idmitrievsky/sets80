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

ErrorCode CreateList(List **list)
{
    List *l = NULL;
    Node *h = NULL, *t = NULL;
    
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
        return ERRORCODE_MEMORY_ALLOCATION_ERROR;
}

ErrorCode AddNode(List **_list, char *content)
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
        return ERRORCODE_MEMORY_ALLOCATION_ERROR;
}

int Length(List *list)
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
    }
    
    return i - 2;
}

Node *Find(List *list, char *content)
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

void DeleteNode(List *list, char *content)
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

void PrintList(List *list)
{
    Node *temp = NULL, *tail = NULL;
    
    tail = list->tail;
    
    temp = list->head->next;
    
    while (temp != tail)
    {
        printf("<%s>\n", temp->content);
        temp = temp->next;
    }
}

void ReleaseList(List **list)
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

int isBefore(Node *node1, Node *node2)
{
    return (strcmp(node1->content, node2->content));
}

Node *nextNode(Node *node)
{
    return node->next;
}

int isTail(List *list, Node *node)
{
    return (node == list->tail);
}






