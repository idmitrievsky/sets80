//
//  main.c
//  Sets
//
//  Created by ivan on 02.04.13.
//  Copyright (c) 2013 dmitrievsky. All rights reserved.
//

#include <stdio.h>
#include "cli.h"
#include "lists.h"
#include "sets.h"

int main(int argc, const char * argv[])
{
    List *cmnds = NULL;
    char string[40];
    
    ErrorCode errorCode = ERRORCODE_NO_ERROR;
    
    Set *A = NULL;
    
    AddToSet(&A, "heelo");
    AddToSet(&A, "bye");
    AddToSet(&A, "heelo");
    AddToSet(&A, "bye");

    DeleteSet(&A);
    
    // do
    {
        ReleaseList(&cmnds);
        scanf("%[^\n]", string);
        CATCH_ERROR(Tokenize(string, &cmnds), errHandler);
        //CATCH_ERROR(Router(cmnds), errHandler);
        PrintList(cmnds);
    } //while (strcmp(cmnds->content, "quit") != 0);
    
    ReleaseList(&cmnds);
    
    return 0;
    
errHandler:
    switch (errorCode)
    {
        case ERRORCODE_QUOTES:
            printf("QUOTES!!!");
            break;
            
        case ERRORCODE_MEMORY_ALLOCATION_ERROR:
            printf("You are out of memory :(");
            break;
            
        case ERRORCODE_DONT_KNOW_COMMAND:
            printf("Command not found :(");
            break;
            
        default:
            break;
    };
    return 1;

}

