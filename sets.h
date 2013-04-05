//
//  sets.h
//  Sets
//
//  Created by ivan on 02.04.13.
//  Copyright (c) 2013 dmitrievsky. All rights reserved.
//

#ifndef Sets_sets_c
#define Sets_sets_c

#include "errors.h"
#include "lists.h"

typedef struct TagSet
{
    List *elems;
    struct TagSet *next;
} Set;

typedef struct TagSetList
{
    Set *head;
    Set *tail;
} SetList;

ErrorCode AddToSet(Set **A, char *string);
ErrorCode DeleteFromSet(Set *A, char *string);
void DeleteSet(Set **set);

#endif
