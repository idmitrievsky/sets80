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
int Cardinality(Set *A);
ErrorCode Union(Set *A, Set *B, Set **C);
ErrorCode Intersection(Set *A, Set *B, Set **C);
ErrorCode SymmDifference(Set *A, Set *B, Set **C);
ErrorCode Substract(Set *A, Set *B, Set **C);
ErrorCode CreateSetWithName(Set **set, char* name);

ErrorCode AddSet(SetList **_setList, Set *set);
ErrorCode AddSetCombination(SetList **_setList, ErrorCode (*Op) (Set *A, Set *B, Set **C), Set *A, Set *B, char *newName);
ErrorCode AddEmptySet(SetList **_setList, char *name);
void ReleaseSetList(SetList **setList);
void DeleteSetFrom(SetList *setList, char *name);
Set *FindInSetList(SetList *setList, char *name);
ErrorCode FindInSetListExact(SetList *setList, char *name, Set **found);
void PrintSetList(SetList *setList);

#endif
