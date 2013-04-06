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
#include "methods.h"
#include "infinitestring.h"

int main(int argc, const char * argv[])
{
    List *cmnds = NULL;
    char *string = NULL;
    
    ErrorCode errorCode = ERRORCODE_NO_ERROR, endOfFile = ERRORCODE_NO_ERROR;
    
    CATCH_ERROR(InitInput(&string), errHandler);
    MEM(string, errHandler);
    
    while (endOfFile == ERRORCODE_NO_ERROR)
    {
        ReleaseList(&cmnds);
        endOfFile = ReadStringFromStream(stdin, &string);
        CATCH_ERROR(Tokenize(string, &cmnds), errHandler);
        CATCH_ERROR(Route(cmnds), errHandler);
        //PrintList(cmnds);
        
        errHandler:
        PrintErrorFeedback(errorCode);
    }
    
    ReleaseList(&cmnds);
    ReleaseInput(string);
    Exit();
    
    return 0;
}

