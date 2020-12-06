/*------------------------------------------------------------------------------
    * File:        StackConfig.h                                               *
    * Description: Stack congigurations                                        *
    * Created:     1 dec 2020                                                  *
    * Copyright:   (C) 2020 MIPT                                               *
    * Author:      Artem Puzankov                                              *
    * Email:       puzankov.ao@phystech.edu                                    *
*///----------------------------------------------------------------------------

#ifndef STACK_CONFIG_H_DEFINED
#define STACK_CONFIG_H_DEFINED


#include <limits.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define CONSOLE_PRINT if(1)
#define DUMP_PRINT    if(1)
static const char* logname = "log.txt";

#define CANARY_PROTECT
#define HASH_PROTECT

#ifndef TYPE
#define TYPE double
#endif // double

#define double_PRINT_FORMAT  "%lf"
#define double_PRINT_TYPE    "double"
#define double_POISON         NAN

#define float_PRINT_FORMAT   "%f"
#define float_PRINT_TYPE     "float"
#define float_POISON          NAN

#define int_PRINT_FORMAT     "%d"
#define int_PRINT_TYPE       "int"
#define int_POISON            INT_MAX

#define char_PRINT_FORMAT    "%c"
#define char_PRINT_TYPE      "char"
#define char_POISON          '\0'


typedef size_t error_t;
typedef unsigned long long can_t;
#define CAN_PRINT_FORMAT "%llu"

static can_t perfect_canary = (srand(time(NULL)), rand());

#define MAX_CAPACITY 100000


enum Errors
{
    OK = 0                                                          ,
    NOT_OK                                                          ,
    NO_MEMORY                                                       ,
    CANARY_DIED                                                     ,
    EMPTY_STACK                                                     ,
    NULL_DATA_PTR                                                   ,
    NULL_STACK_PTR                                                  ,
    INCORRECT_HASH                                                  ,
    STACK_DESTRUCTED                                                ,
    SIZE_BIGGER_CAPACITY                                            ,
    CAPACITY_WRONG_VALUE                                            ,
};

static const char* errstr[] =
{
    "OK"                                                            ,
    "ERROR"                                                         ,
    "\nFailed to allocate memory\n"                                 ,
    "\nStack cracked, canary was killed\n\n"                        ,
    "\nStack is empty\n"                                            ,
    "\nThe pointer to the stack is null, data lost\n\n"             ,
    "\nThe pointer to the stack is null, stack lost\n\n"            ,
    "\nStack cracked, hash corrupted\n\n"                           ,
    "\nStack already destructed\n"                                  ,
    "\nThe size of the stack data is larger than the capacity\n\n"  ,
    "\nBad size stack capacity\n\n"                                 ,
};

#endif // STACK_CONFIG_H_DEFINED
