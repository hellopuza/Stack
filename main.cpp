/*------------------------------------------------------------------------------
    * File:        main.cpp                                                    *
    * Description: Program uses safely stack                                   *
    * Author:      Artem Puzankov                                              *
    * Email:       puzankov.ao@phystech.edu                                    *
    * GitHub:      https://github.com/hellopuza                                *
    * Copyright © 2021 Artem Puzankov. All rights reserved.                    *
    *///------------------------------------------------------------------------

#define _CRT_SECURE_NO_WARNINGS

#include "Stack.h"

int main()
{
    StackConstruct(stk, 8, double);
    
    stk.Push(10.1);
    stk.Push(-12.2);
    stk.Push(11.22552e-2);
    stk.Push(-0.4);
    stk.Push(12);

    printf("%lf\n", stk.Pop());
    printf("%lf\n", stk.Pop());
    printf("%lf\n", stk.Pop());
    printf("%lf\n", stk.Pop());
    printf("%lf\n", stk.Pop());
    printf("%lf\n", stk.Pop());
    printf("%lf\n", stk.Pop());


    return 0;
}

