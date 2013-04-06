//
//  errors.c
//  Sets
//
//  Created by ivan on 02.04.13.
//  Copyright (c) 2013 dmitrievsky. All rights reserved.
//

#include <stdio.h>
#include "errors.h"

#define X(a, b) b,
static char *errorTexts[] =
{
#include "errors.txt"
};
#undef X

void PrintErrorFeedback(ErrorCode errorCode)
{
    printf("%s", errorTexts[errorCode]);
}

void PrintMethodsHelp(char *string)
{
    printf("%s\n", string);
}