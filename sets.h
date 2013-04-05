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
    char *name;
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

int IsSubset(Set *A, Set *B);
int IsMember(char *string, Set *A);
ErrorCode Union(Set *A, Set *B, Set **C);
ErrorCode Intersection(Set *A, Set *B, Set **C);
ErrorCode SymmDifference(Set *A, Set *B, Set **C);
ErrorCode Substract(Set *A, Set *B, Set **C);
ErrorCode CreateSetWithName(Set **set, char* name);

#endif
