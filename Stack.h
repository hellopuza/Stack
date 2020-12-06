/*------------------------------------------------------------------------------
    * File:        Stack.h                                                     *
    * Description: Main stack functions                                        *
    * Created:     1 dec 2020                                                  *
    * Copyright:   (C) 2020 MIPT                                               *
    * Author:      Artem Puzankov                                              *
    * Email:       puzankov.ao@phystech.edu                                    *
*///----------------------------------------------------------------------------

#define _CRT_SECURE_NO_WARNINGS


#include "StackConfig.h"
#include "Template.h"
#include <windows.h>
#include <assert.h>
#include <limits.h>
#include <memory.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef HASH_PROTECT
#include "hash.h"
#endif // HASH_PROTECT


#define ASSERTOK if (TEMPLATE(StackCheck, TYPE) (p_stk, __FUNCTION__))                                           \
                 {                                                                                               \
                     FILE* log = fopen(logname, "a");                                                            \
                     assert (log != nullptr);                                                                    \
                     fprintf(log, "ERROR: file %s  line %d  function %s\n\n", __FILE__, __LINE__, __FUNCTION__); \
                     printf (     "ERROR: file %s  line %d  function %s\n",   __FILE__, __LINE__, __FUNCTION__); \
                     fclose(log);                                                                                \
                     TEMPLATE(StackDump, TYPE) (p_stk, __FUNCTION__, logname);                                   \
                     exit(p_stk->errCode);/**/                                                                   \
                 }


static char* stack_name = nullptr;

#define StackConstruct_double(p_stk, capacity)                                                                   \
        stack_name = (char*)#p_stk;                                                                              \
        TEMPLATE(_StackConstruct, double) (p_stk, capacity, ++stack_name);                                       \

#define StackConstruct_float(p_stk, capacity)                                                                    \
        stack_name = (char*)#p_stk;                                                                              \
        TEMPLATE(_StackConstruct, float) (p_stk, capacity, ++stack_name);                                        \

#define StackConstruct_int(p_stk, capacity)                                                                      \
        stack_name = (char*)#p_stk;                                                                              \
        TEMPLATE(_StackConstruct, int) (p_stk, capacity, ++stack_name);                                          \

#define StackConstruct_char(p_stk, capacity)                                                                     \
        stack_name = (char*)#p_stk;                                                                              \
        TEMPLATE(_StackConstruct, char) (p_stk, capacity, ++stack_name);                                         \


typedef struct Stack
{
//////////////TRY-TO-HACK///////////////
    can_t canary1;

    size_t  capacity;
    size_t  size_cur;
    const char* name;

    TYPE* data;
    
#ifdef HASH_PROTECT
    hash_t* stackhash;
    hash_t* datahash;
#endif // HASH_PROTECT
    
    int errCode;
    
    can_t canary2;
//////////////TRY-TO-HACK///////////////
} TEMPLATE(stack, TYPE);


//------------------------------------------------------------------------------
/*! @brief   Stack constructor
 *
 *  @param   p_stk       Pointer to stack
 *  @param   capacity    Capacity of stack
 *  @param   stack_name  Stack variable name
 *
 *  @return  code error
 */

error_t TEMPLATE(_StackConstruct, TYPE) (TEMPLATE(stack, TYPE)* p_stk, size_t capacity, char* stack_name);

//------------------------------------------------------------------------------
/*! @brief   Stack destructor
 *
 *  @param   p_stk       Pointer to stack
 *
 *  @return  code error
 */

error_t TEMPLATE(StackDestruct, TYPE) (TEMPLATE(stack, TYPE)* p_stk);

//------------------------------------------------------------------------------
/*! @brief   Pushing a value onto the stack
 *
 *  @param   p_stk       Pointer to stack
 *  @param   value       Value to push
 *
 *  @return  code error
 */

error_t TEMPLATE(StackPush, TYPE) (TEMPLATE(stack, TYPE)* p_stk, TYPE value);

//------------------------------------------------------------------------------
/*! @brief   Popping from stack
 *
 *  @param   p_stk       Pointer to stack
 *
 *  @return  value from the stack if present, otherwise POISON
 */

TYPE TEMPLATE(StackPop, TYPE) (TEMPLATE(stack, TYPE)* p_stk);

//------------------------------------------------------------------------------
/*! @brief   Filling the data stack with POISON
 *
 *  @param   p_stk       Pointer to stack
 */

void TEMPLATE(StackPoison, TYPE) (TEMPLATE(stack, TYPE)* p_stk);

//------------------------------------------------------------------------------
/*! @brief   Increase the stack by 2 times
 *
 *  @param   p_stk       Pointer to stack
 *
 *  @return  error code
 */

error_t TEMPLATE(StackExpand, TYPE) (TEMPLATE(stack, TYPE)* p_stk);

//------------------------------------------------------------------------------
/*! @brief   (THE BEST FUNCTION) Print the contents of the stack and its data to the logfile
 *
 *  @param   p_stk       Pointer to stack
 *  @param   funcname    Name of the function from which the StackDump was called
 *  @param   logname     Name of logfile
 *
 *  @return  error code
 */

error_t TEMPLATE(StackDump, TYPE) (TEMPLATE(stack, TYPE)* p_stk, const char* funcname = "@some function@", const char* logfile = logname);

//------------------------------------------------------------------------------
/*! @brief   �heck stack for problems, canaries, hash (if enabled)
 *
 *  @param   p_stk       Pointer to stack
 *  @param   funcname    Name of the function from which the StackCheck was called
 *
 *  @return  error code
 */

error_t TEMPLATE(StackCheck, TYPE) (TEMPLATE(stack, TYPE)* p_stk, const char* funcname);

//------------------------------------------------------------------------------
/*! @brief   Print information, error summary
 *
 *  @param   p_stk       Pointer to stack
 *  @param   fp          Pointer to logfile
 *
 *  @return  error code
 */

void TEMPLATE(printError, TYPE) (TEMPLATE(stack, TYPE)* p_stk, FILE* fp);

//------------------------------------------------------------------------------
/*! @brief   Change canary (if enabled)
 *
 *  @param   canary      �anary input
 *
 *  @return  changed canary
 */

#ifdef CANARY_PROTECT

can_t CanaryChange(can_t canary);

#endif // CANARY_PROTECT

//------------------------------------------------------------------------------
/*! @brief   Placing canaries in stack (if enabled)
 *
 *  @param   p_stk       Pointer to stack
 */

#ifdef CANARY_PROTECT

void TEMPLATE(CanaryPlacing, TYPE) (TEMPLATE(stack, TYPE)* p_stk);

#endif // CANARY_PROTECT

//------------------------------------------------------------------------------
/*! @brief   Check stack for canaries (if enabled)
 *
 *  @param   p_stk       Pointer to stack
 *  @param   funcname    Name of the function from which the StackCheck was called
 *
 *  @return  OK if all canaries aren't damaged, otherwise NOT_OK
 */

#ifdef CANARY_PROTECT

int TEMPLATE(CanaryCheck, TYPE) (TEMPLATE(stack, TYPE)* p_stk);

#endif // CANARY_PROTECT

//------------------------------------------------------------------------------
