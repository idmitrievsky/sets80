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
    s->elems = NULL;
    s->next = NULL;
    *set = s;
    
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
    
    if (!Find(set->elems, string))
    {
        CATCH_ERROR(AddNode(&(set->elems), string), errHandler);
    }
    
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
    free(*set);
    *set = NULL;
}





