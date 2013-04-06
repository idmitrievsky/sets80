//
//  methods.c
//  Sets
//
//  Created by ivan on 06.04.13.
//  Copyright (c) 2013 dmitrievsky. All rights reserved.
//

#include <stdlib.h>
#include <string.h>
#include "errors.h"
#include "lists.h"
#include "sets.h"
#include "methods.h"

static SetList *ListOfSets = NULL;

typedef ErrorCode (*Method) (List *list);

#define X(a, b, c, d) a,
typedef enum TagCommands
{
#include "methods.txt"
} Commands;
#undef X

#define X(a, b, c, d) b,
char *commands_names[] =
{
#include "methods.txt"
};
#undef X

ErrorCode Create(List *args)
{
    ErrorCode errorCode = ERRORCODE_NO_ERROR;
    
    if (Length(args) != 1)
    {
        return ERRORCODE_WRONG_NUMBER_OF_ARGS;
    }
    CATCH_ERROR(AddEmptySet(&ListOfSets, args->head->next->content), errHandler);
    
    return ERRORCODE_NO_ERROR;
    
errHandler:
    return errorCode;
}

ErrorCode Delete(List *list)
{
    return ERRORCODE_NO_ERROR;
}
ErrorCode Empty(List *list)
{
    return ERRORCODE_NO_ERROR;
}

#define X(a, b, c, d) c,
Method methods[] =
{
#include "methods.txt"
};
#undef X

ErrorCode Route(List *list)
{
    ErrorCode errorCode = ERRORCODE_NO_ERROR;
    Commands command = 0;
    char *methodName = NULL;
    List *args = NULL;
    
    methodName = list->head->next->content;
    
    while (strcmp(methodName, commands_names[command]) & strcmp("", commands_names[command]))
    {
        command++;
    }
    
    if (!strcmp("", commands_names[command]))
    {
        return ERRORCODE_DONT_KNOW_COMMAND;
    }
    else
    {
        CATCH_ERROR(AppendToFrom(&args, list->head->next->next, list), errHandler);
        CATCH_ERROR(methods[command](args), errHandler);
        ReleaseList(&args);
    }
    
    return ERRORCODE_NO_ERROR;
    
errHandler:
    if (args)
    {
        ReleaseList(&args);
    }
    return errorCode;
}

void Exit()
{
    ReleaseSetList(&ListOfSets);
}
