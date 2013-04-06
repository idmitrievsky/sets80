//
//  infinitestring.h
//  Sets
//
//  Created by ivan on 06.04.13.
//  Copyright (c) 2013 dmitrievsky. All rights reserved.
//

#ifndef Sets_infinitestring_c
#define Sets_infinitestring_c

#include <stdio.h>
#include "errors.h"

#define STREAM_STRING_SIZE 20 // initialistion size for stringForInput (its size always multiples of STREAM_STRING_SIZE)

ErrorCode ReadStringFromStream(FILE *stream, char **pointerToStringForInput);

#endif
