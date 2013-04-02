//
//  main.c
//  Sets
//
//  Created by ivan on 02.04.13.
//  Copyright (c) 2013 dmitrievsky. All rights reserved.
//

#include <stdio.h>
#include "lists.h"

int main(int argc, const char * argv[])
{
    List *list = NULL;
    
    AddNode(&list, "aaa");
    AddNode(&list, "aab");
    AddNode(&list, "aca");
    DeleteNode(list, "acab");
    PrintList(list);
    ReleaseList(&list);
    return 0;
}

