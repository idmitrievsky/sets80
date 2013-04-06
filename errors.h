//
//  errors.h
//  Sets
//
//  Created by ivan on 02.04.13.
//  Copyright (c) 2013 dmitrievsky. All rights reserved.
//

#ifndef Sets_errors_c
#define Sets_errors_c

#define MAX_ERROR_MESSAGE_SIZE 60 // ???
#define CATCH_ERROR(e, label) { ErrorCode cearg = e; errorCode = cearg; if (cearg != ERRORCODE_NO_ERROR) { goto label; } else {}}
#define MEM(pointer, label) {if (pointer == NULL) { goto label; } else {}}

#define X(a, b) a,
typedef enum
{
#include "errors.txt"
} ErrorCode;
#undef X

void PrintErrorFeedback(ErrorCode errorCode);
void PrintMethodsHelp(char *string);

#endif
