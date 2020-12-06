/*------------------------------------------------------------------------------
    * File:        main.cpp                                                    *
    * Description: Program uses safely stack                                   *
    * Created:     1 dec 2020                                                  *
    * Copyright:   (C) 2020 MIPT                                               *
    * Author:      Artem Puzankov                                              *
    * Email:       puzankov.ao@phystech.edu                                    *
*///----------------------------------------------------------------------------
// 972 lines

#define _CRT_SECURE_NO_WARNINGS

#define TYPE double
#include "Stack.h"
#undef TYPE

int main()
{
    stack_double stk = {};
    StackConstruct_double(&stk, 8);

    StackPush_double(&stk, 10.1);
    StackPush_double(&stk, -12.2);
    StackPush_double(&stk, 11.22552e-2);
    StackPush_double(&stk, -0.4);
    StackPush_double(&stk, 12);
    StackPush_double(&stk, 10.3);
    StackPush_double(&stk, -12.25);
    StackPush_double(&stk, 11.11);

    StackPop_double(&stk);
    StackPop_double(&stk);
    StackPop_double(&stk);

    stk.size_cur = 2;

    StackPop_double(&stk);
    StackPop_double(&stk);

    StackDestruct_double(&stk);


    printf("    /|    /|    /|\n"
           "   //|   //|   //|\n"
           "  //-|  //-|  //-|\n"
           " // 0| // 0| // 0|\n"
           "//===|//===|//===|\n"
           "| STACK IS GOOD! |\n");


    return 0;
}
