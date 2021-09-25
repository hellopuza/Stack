/*------------------------------------------------------------------------------
    * File:        main.cpp                                                    *
    * Description: Program uses c++ stack                                      *
    * Author:      Artem Puzankov                                              *
    * Email:       puzankov.ao@phystech.edu                                    *
    * GitHub:      https://github.com/hellopuza                                *
    * Copyright © 2021 Artem Puzankov. All rights reserved.                    *
    *///------------------------------------------------------------------------

#define _CRT_SECURE_NO_WARNINGS

#include "StackLib/Stack.h"

int main()
{
    newStack_size(stk, 4, double);
    //newStack_size(stk2, 4, double);

    stk.Push(10.1);
    stk.Push(-12.2);
    stk.Pop();
    stk.Push(11.22552e-2);
    stk.Push(-0.4);

    Stack<double> stk2 = stk;
    //stk2 = Stack<double>((char*)"aa", 12);
    stk2.Dump();
    stk.Dump();

    return 0;
}

