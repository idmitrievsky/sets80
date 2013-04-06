//
//  lists.h
//  Sets
//
//  Created by ivan on 02.04.13.
//  Copyright (c) 2013 dmitrievsky. All rights reserved.
//

#ifndef Sets_lists_c
#define Sets_lists_c

#include "errors.h"

typedef struct TagNode
{
    char *content;
    struct TagNode *next;
} Node;

typedef struct TagList
{
    Node *head;
    Node *tail;
} List;

ErrorCode CreateList(List **list);
ErrorCode AddNode(List **_list, char *content);
ErrorCode AddNodeToEndForced(List **_list, char *content);
int Length(List *list);
Node *Find(List *list, char *content);
void DeleteNode(List *list, char *content);
void PrintList(List *list);
void ReleaseList(List **list);
int IsBefore(Node *node1, Node *node2);
Node *NextNode(Node *node);
int IsTailOf(Node *node, List *list);
ErrorCode AppendToFrom(List **to, Node *start, List *from);

#endif
