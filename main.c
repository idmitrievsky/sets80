//
//  main.c
//  Sets
//
//  Created by ivan on 02.04.13.
//  Copyright (c) 2013 dmitrievsky. All rights reserved.
//

#include "cli.h"
#include "lists.h"
#include "sets.h"
#include "methods.h"
#include "infinitestring.h"

int main(int argc, const char * argv[])
{
    List *cmnds = NULL;
    char *string = NULL;
    int quit = 0;
    
    ErrorCode errorCode = ERRORCODE_NO_ERROR, endOfFile = ERRORCODE_NO_ERROR;
    
    CATCH_ERROR(InitInput(&string), errHandler); // allocating input string
    MEM(string, initialError); // checking if allocation was successful, otherwise quiting
    
    while (endOfFile == ERRORCODE_NO_ERROR && quit != 1) // ends on <quit> command
    {
        ReleaseList(&cmnds); // releasing list of tokens
        endOfFile = ReadStringFromStream(stdin, &string); // reading string of unknown length
        CATCH_ERROR(Tokenize(string, &cmnds), errHandler); // creating a list of tokens from input string
        CATCH_ERROR(Route(cmnds, &quit), errHandler); // trying to find a matching method
        
        errHandler:
            PrintErrorFeedback(errorCode); // on error printing feedback info and continuing main loop
    }
    
    ReleaseList(&cmnds); // after escaping while
    ReleaseInput(string);
    Exit(); // releasing all allocated sets
    
    return 0;
    
    initialError:
        return 1;
}

