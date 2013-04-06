//
//  infinitestring.c
//  Sets
//
//  Created by ivan on 06.04.13.
//  Copyright (c) 2013 dmitrievsky. All rights reserved.
//

#include <stdlib.h>
#include "errors.h"
#include "infinitestring.h"

void AppendChar(char **tail, int chr);

ErrorCode ReadStringFromStream(FILE *stream, char **pointerToStringForInput)
{
    int c, overloadCount = 1; // overlaodCount counts how many times the stringForInput changing its size
    char *tail;
    char *temp;
    
    *pointerToStringForInput[0] = '\0'; // initialisation with '\0' (after it stringForInput is empty)
    tail = *pointerToStringForInput; //  every time function is called tail points to the start of the stringForInput
    
    /*
     Proper exit from this "while" is possible only if EOF is reached
     In another cases it's provided by "break" (if carriage return is reached) or "return" (if there was an error)
     */
    while ((c = fgetc(stream)) != EOF && c != 26)
    {
        if (tail >= *pointerToStringForInput + overloadCount*(STREAM_STRING_SIZE - 1) + (overloadCount - 1)) // is there not enough space for writing yet another symbol?
        {
            overloadCount++;
            if ((temp = realloc(*pointerToStringForInput, STREAM_STRING_SIZE * overloadCount)) == NULL) // attempt to allocate more memory
            {
                return ERRORCODE_MEMORY_ALLOCATION_ERROR;
            }
            *pointerToStringForInput = temp;
            tail = *pointerToStringForInput + (overloadCount - 1) * (STREAM_STRING_SIZE - 1) + (overloadCount - 2); // if realloc result was a different pointer, tail needs to be changed
        }
        if (c == '\n')
        {
            return ERRORCODE_NO_ERROR;
        }
        AppendChar(&tail, c); // test for carriage return is before reallocing, so stringForInput doesn't have '\n' at the end
    }
    
    return ERRORCODE_EOF; // only EOF or 26 goes here
}

void AppendChar(char **tail, int c)
{
    **tail = (char)c;
    *tail += 1;
    **tail = '\0';
}

ErrorCode InitInput(char **string)
{
    *string = malloc(STREAM_STRING_SIZE);
    MEM(string, errHandler);
    
    return ERRORCODE_NO_ERROR;
    
errHandler:
    return ERRORCODE_MEMORY_ALLOCATION_ERROR;
}

void ReleaseInput(char *string)
{
    free(string);
}


