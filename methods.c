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

#define X(a, b, c, d) d,
char *helpNotes[] =
{
#include "methods.txt"
};
#undef X

ErrorCode MCreate(List *args)
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

ErrorCode MDelete(List *args)
{
    if (Length(args) != 1)
    {
        return ERRORCODE_WRONG_NUMBER_OF_ARGS;
    }
    DeleteSetFrom(ListOfSets, args->head->next->content);
    
    return ERRORCODE_NO_ERROR;
}

ErrorCode MEmpty(List *list)
{
    return ERRORCODE_EMPTY_COMMAND;
}

ErrorCode MEnd(List *list)
{
    return ERRORCODE_NO_ERROR;
}

ErrorCode MView(List *args)
{
    Set *found = NULL;
    
    if (Length(args) == 0)
    {
        PrintSetList(ListOfSets);
        return ERRORCODE_NO_ERROR;
    }
    
    if (Length(args) == 1)
    {
        if ((found = FindInSetList(ListOfSets, args->head->next->content)))
        {
            PrintList(found->next->elems);
            return ERRORCODE_NO_ERROR;
        }
        else
        {
            return ERRORCODE_NO_SUCH_SET;
        }
        
    }
    
    return ERRORCODE_WRONG_NUMBER_OF_ARGS;
}

ErrorCode MCard(List *args)
{
    ErrorCode errorCode = ERRORCODE_NO_ERROR;
    Set *found = NULL;
    
    if (Length(args) == 1)
    {
        CATCH_ERROR(FindInSetListExact(ListOfSets, args->head->next->content, &found), errHandler);
        Cardinality(found);
        return ERRORCODE_NO_ERROR;
    }
    
    return ERRORCODE_WRONG_NUMBER_OF_ARGS;
    
errHandler:
    return errorCode;
}

ErrorCode MAdd(List *args)
{
    ErrorCode errorCode = ERRORCODE_NO_ERROR;
    char *name = NULL, *element = NULL;
    Set *found = NULL;
    
    if (Length(args) == 2)
    {
        element = args->head->next->content;
        name = args->head->next->next->content;
        CATCH_ERROR(FindInSetListExact(ListOfSets, name, &found), errHandler);
        CATCH_ERROR(AddToSet(&found, element), errHandler);
        return ERRORCODE_NO_ERROR;
    }
    
    return ERRORCODE_WRONG_NUMBER_OF_ARGS;
    
errHandler:
    return errorCode;
}

ErrorCode MRemove(List *args)
{
    ErrorCode errorCode = ERRORCODE_NO_ERROR;
    char *name = NULL, *element = NULL;
    Set *found = NULL;
    
    if (Length(args) == 2)
    {
        element = args->head->next->content;
        name = args->head->next->next->content;
        CATCH_ERROR(FindInSetListExact(ListOfSets, name, &found), errHandler);
        CATCH_ERROR(DeleteFromSet(found, element), errHandler);
        return ERRORCODE_NO_ERROR;
    }
    
    return ERRORCODE_WRONG_NUMBER_OF_ARGS;
    
errHandler:
    return errorCode;
}

ErrorCode MBelongs(List *args)
{
    ErrorCode errorCode = ERRORCODE_NO_ERROR;
    char *name = NULL, *element = NULL;
    Set *found = NULL;
    
    if (Length(args) == 2)
    {
        element = args->head->next->content;
        name = args->head->next->next->content;
        CATCH_ERROR(FindInSetListExact(ListOfSets, name, &found), errHandler);
        IsMember(element, found);
        return ERRORCODE_NO_ERROR;
    }
    
    return ERRORCODE_WRONG_NUMBER_OF_ARGS;
    
errHandler:
    return errorCode;
}

ErrorCode MUnion(List *args)
{
    ErrorCode errorCode = ERRORCODE_NO_ERROR;
    char *name1 = NULL, *name2 = NULL, *newName = NULL;
    Set *found1 = NULL, *found2 = NULL;
    
    
    if (Length(args) == 3)
    {
        name1 = args->head->next->content;
        name2 = args->head->next->next->content;
        newName = args->head->next->next->next->content;
        CATCH_ERROR(FindInSetListExact(ListOfSets, name1, &found1), errHandler);
        CATCH_ERROR(FindInSetListExact(ListOfSets, name2, &found2), errHandler);
        if (FindInSetList(ListOfSets, newName))
        {
            return ERRORCODE_ALREADY_EXISTS;
        }
        CATCH_ERROR(AddSetCombination(&ListOfSets, Union, found1, found2, newName), errHandler);
        return ERRORCODE_NO_ERROR;
    }
    
    return ERRORCODE_WRONG_NUMBER_OF_ARGS;
    
errHandler:
    return errorCode;
}

ErrorCode MIntersection(List *args)
{
    ErrorCode errorCode = ERRORCODE_NO_ERROR;
    char *name1 = NULL, *name2 = NULL, *newName = NULL;
    Set *found1 = NULL, *found2 = NULL;
    
    
    if (Length(args) == 3)
    {
        name1 = args->head->next->content;
        name2 = args->head->next->next->content;
        newName = args->head->next->next->next->content;
        CATCH_ERROR(FindInSetListExact(ListOfSets, name1, &found1), errHandler);
        CATCH_ERROR(FindInSetListExact(ListOfSets, name2, &found2), errHandler);
        if (FindInSetList(ListOfSets, newName))
        {
            return ERRORCODE_ALREADY_EXISTS;
        }
        CATCH_ERROR(AddSetCombination(&ListOfSets, Intersection, found1, found2, newName), errHandler);
        return ERRORCODE_NO_ERROR;
    }
    
    return ERRORCODE_WRONG_NUMBER_OF_ARGS;
    
errHandler:
    return errorCode;
}

ErrorCode MSDifference(List *args)
{
    ErrorCode errorCode = ERRORCODE_NO_ERROR;
    char *name1 = NULL, *name2 = NULL, *newName = NULL;
    Set *found1 = NULL, *found2 = NULL;
    
    
    if (Length(args) == 3)
    {
        name1 = args->head->next->content;
        name2 = args->head->next->next->content;
        newName = args->head->next->next->next->content;
        CATCH_ERROR(FindInSetListExact(ListOfSets, name1, &found1), errHandler);
        CATCH_ERROR(FindInSetListExact(ListOfSets, name2, &found2), errHandler);
        if (FindInSetList(ListOfSets, newName))
        {
            return ERRORCODE_ALREADY_EXISTS;
        }
        CATCH_ERROR(AddSetCombination(&ListOfSets, SymmDifference, found1, found2, newName), errHandler);
        return ERRORCODE_NO_ERROR;
    }
    
    return ERRORCODE_WRONG_NUMBER_OF_ARGS;
    
errHandler:
    return errorCode;
}

ErrorCode MSubstract(List *args)
{
    ErrorCode errorCode = ERRORCODE_NO_ERROR;
    char *name1 = NULL, *name2 = NULL, *newName = NULL;
    Set *found1 = NULL, *found2 = NULL;
    
    
    if (Length(args) == 3)
    {
        name1 = args->head->next->content;
        name2 = args->head->next->next->content;
        newName = args->head->next->next->next->content;
        CATCH_ERROR(FindInSetListExact(ListOfSets, name1, &found1), errHandler);
        CATCH_ERROR(FindInSetListExact(ListOfSets, name2, &found2), errHandler);
        if (FindInSetList(ListOfSets, newName))
        {
            return ERRORCODE_ALREADY_EXISTS;
        }
        CATCH_ERROR(AddSetCombination(&ListOfSets, Substract, found1, found2, newName), errHandler);
        return ERRORCODE_NO_ERROR;
    }
    
    return ERRORCODE_WRONG_NUMBER_OF_ARGS;
    
errHandler:
    return errorCode;
}

ErrorCode MContains(List *args)
{
    ErrorCode errorCode = ERRORCODE_NO_ERROR;
    char *name1 = NULL, *name2 = NULL;
    Set *found1 = NULL, *found2 = NULL;
    
    
    if (Length(args) == 2)
    {
        name1 = args->head->next->content;
        name2 = args->head->next->next->content;
        CATCH_ERROR(FindInSetListExact(ListOfSets, name1, &found1), errHandler);
        CATCH_ERROR(FindInSetListExact(ListOfSets, name2, &found2), errHandler);
        IsSubset(found2, found1);
        return ERRORCODE_NO_ERROR;
    }
    
    return ERRORCODE_WRONG_NUMBER_OF_ARGS;
    
errHandler:
    return errorCode;
}

ErrorCode MQuit(List *args)
{
    return ERRORCODE_NO_ERROR;
}

ErrorCode MHelp(List *args)
{
    Commands command = 0;
    
    if (Length(args) == 0)
    {
        while (strcmp(commands_names[EMPTY], commands_names[command]))
        {
            PrintMethodsHelp(helpNotes[command]);
            command++;
        }
        return ERRORCODE_NO_ERROR;
    }
    return ERRORCODE_WRONG_NUMBER_OF_ARGS;
}

#define X(a, b, c, d) c,
Method methods[] =
{
#include "methods.txt"
};
#undef X

ErrorCode Route(List *list, int *quit)
{
    ErrorCode errorCode = ERRORCODE_NO_ERROR;
    Commands command = 0;
    char *methodName = NULL;
    List *args = NULL;
    
    methodName = list->head->next->content;
    
    while (strcmp(commands_names[END], commands_names[command]) && strcmp(methodName, commands_names[command]))
    {
        command++;
    }
    
    if (!strcmp(commands_names[END], commands_names[command]))
    {
        return ERRORCODE_DONT_KNOW_COMMAND;
    }
    else if (!strcmp(commands_names[QUIT], commands_names[command]))
    {
        *quit = 1;
    }
    else
    {
        CATCH_ERROR(AppendToFromForced(&args, list->head->next->next, list), errHandler);
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
