//
//  cli.c
//  Sets
//
//  Created by ivan on 02.04.13.
//  Copyright (c) 2013 dmitrievsky. All rights reserved.
//

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "cli.h"

ErrorCode Tokenize(char *string, List **result)
{
    int i     = 0,
    space = 1, // is last symbol a space?
    quote = 0; // is last quote unmatched?
    
    char *temp  = NULL, // pointer for malloc
    *begin = NULL; // first letter in current "word"
    
    ErrorCode errorCode = ERRORCODE_NO_ERROR; //
    
    *result = NULL;
    
    for (i = 0; i < strlen(string); i++)
    {
        if (string[i] == '"')
        {
            if (quote == 0) // opening quote?
            {
                if (space == 1) // quote following non-space symbol is not allowed
                {
                    begin = string + i + 1; // omitting an opening quote is result string
                    space = 0;
                }
                else
                {
                    return ERRORCODE_QUOTES;
                }
                quote = 1; // now we are inside quotes
            }
            else // closing quote
            {
                quote = 0;
                continue; // without 'continue' quote symbol will be processed as a non-space symbol below
            }
        }
        
        if (quote == 0) // we are tracking spaces only if we are not inside quotes
        {
            if (isspace(string[i]))
            {
                if (space == 0) // checking if it is the end of a word
                {
                    temp = malloc(string + i - begin + 1); // extra byte for a closing 0
                    MEM(temp, errHandler);
                    memcpy(temp, begin, string + i - begin ); // copying word
                    temp[string + i - begin] = '\0'; // terminating string
                    // if this space is just after closing quote then we can delete that
                    if (temp[string + i - begin - 1] == '"') { temp[string + i - begin - 1] = '\0'; }
                    CATCH_ERROR(AddNode(result, temp), errHandler);
                }
                space = 1; // we can be in a sequence of spaces
            }
            else
            {
                if (space == 1) // non-space symbol after a space = begining of a new word
                {
                    begin = string + i;
                }
                if (i > 0 && string[i - 1] == '"') // letter can't follow a quote outside quotes
                {
                    return ERRORCODE_QUOTES;
                }
                space = 0;
            }
        }
        temp = NULL;
    }
    
    if (string[strlen(string) - 1] != ' ') // if the whole string ends with a non-space symbol, then one word left we need to count
    {
        temp = malloc(string + i - begin + 1);
        MEM(temp, errHandler);
        memcpy(temp, begin, string + i - begin );
        temp[string + i - begin] = '\0';
        if (temp[string + i - begin - 1] == '"') { temp[string + i - begin - 1] = '\0'; }
        CATCH_ERROR(AddNode(result, temp), errHandler);
    }
    
    // if quotes doesn't match, then final word is from last opening quote 'til the end of a sting
    
    return ERRORCODE_NO_ERROR;
    
errHandler:
    if (temp != NULL) {
        free(temp);
    }
    return ERRORCODE_MEMORY_ALLOCATION_ERROR;
}

//ErrorCode Router(List *list)
//{
//    Method method = NULL;
//    char *string;
//    ErrorCode errorCode = ERRORCODE_NO_ERROR;
//    
//    string = list->content;
//    
//    if ((method = FindMatch(string)))
//    {
//        CATCH_ERROR(method(list->next), errHandler);
//    }
//    
//    return ERRORCODE_NO_ERROR;
//    
//errHandler:
//    return errorCode;
//}
