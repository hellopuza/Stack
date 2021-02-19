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


#define ASSERTOK(p_stk) if (TEMPLATE(StackCheck, TYPE) (p_stk, __FUNCTION__))                                         \
                        {                                                                                             \
                          FILE* log = fopen(logname, "a");                                                            \
                          assert (log != nullptr);                                                                    \
                          fprintf(log, "ERROR: file %s  line %d  function %s\n\n", __FILE__, __LINE__, __FUNCTION__); \
                          printf (     "ERROR: file %s  line %d  function %s\n",   __FILE__, __LINE__, __FUNCTION__); \
                          fclose(log);                                                                                \
                          TEMPLATE(StackDump, TYPE) (p_stk, __FUNCTION__, logname);                                   \
                          exit(p_stk->errCode); /**/                                                                  \
                        }


#ifndef STACK_ID
static int stack_id = 0;
#define STACK_ID
#endif // STACK_ID


#ifndef STACK_NAME
static char* stack_name = nullptr;
#define STACK_NAME
#endif // STACK_NAME

#define StackConstruct(NAME, capacity, STK_TYPE)                              \
        TEMPLATE(stack, STK_TYPE) NAME;                                       \
        TEMPLATE(_StackConstruct, STK_TYPE) (&##NAME, capacity, (char*)#NAME);


typedef struct TEMPLATE(Stack, TYPE)
{
//////////////TRY-TO-HACK///////////////
    can_t canary1;

    size_t  capacity;
    size_t  size_cur;
    const char* name;

    TYPE* data;

    int errCode = NOT_CONSTRUCTED;
    int id;

#ifdef HASH_PROTECT
    hash_t stackhash;
    hash_t datahash;
#endif // HASH_PROTECT

    can_t canary2;
//////////////TRY-TO-HACK///////////////
} TEMPLATE(stack, TYPE);


//==============================================================================
/*------------------------------------------------------------------------------
                   StackLib definitions                                        *
*///----------------------------------------------------------------------------
//==============================================================================

//------------------------------------------------------------------------------
/*! @brief   Stack constructor
 *
 *  @param   p_stk       Pointer to stack
 *  @param   capacity    Capacity of stack
 *  @param   stack_name  Stack variable name
 *
 *  @return  code error
 */

static error_t TEMPLATE(_StackConstruct, TYPE) (TEMPLATE(stack, TYPE)* p_stk, size_t capacity, char* stack_name);

//------------------------------------------------------------------------------
/*! @brief   Stack destructor
 *
 *  @param   p_stk       Pointer to stack
 *
 *  @return  code error
 */

static error_t TEMPLATE(StackDestruct, TYPE) (TEMPLATE(stack, TYPE)* p_stk);

//------------------------------------------------------------------------------
/*! @brief   Pushing a value onto the stack
 *
 *  @param   p_stk       Pointer to stack
 *  @param   value       Value to push
 *
 *  @return  code error
 */

static error_t TEMPLATE(StackPush, TYPE) (TEMPLATE(stack, TYPE)* p_stk, TYPE value);

//------------------------------------------------------------------------------
/*! @brief   Popping from stack
 *
 *  @param   p_stk       Pointer to stack
 *
 *  @return  value from the stack if present, otherwise POISON
 */

static TYPE TEMPLATE(StackPop, TYPE) (TEMPLATE(stack, TYPE)* p_stk);

//------------------------------------------------------------------------------
/*! @brief   Filling the data stack with POISON
 *
 *  @param   p_stk       Pointer to stack
 */

static void TEMPLATE(StackPoison, TYPE) (TEMPLATE(stack, TYPE)* p_stk);

//------------------------------------------------------------------------------
/*! @brief   Check if value is POISON
 *
 *  @param   value       Checked value
 *
 *  @return 1 if value is POISON, else 0
 */

static int TEMPLATE(isPOISON, TYPE) (TYPE value);

//------------------------------------------------------------------------------
/*! @brief   Increase the stack by 2 times
 *
 *  @param   p_stk       Pointer to stack
 *
 *  @return  error code
 */

static error_t TEMPLATE(StackExpand, TYPE) (TEMPLATE(stack, TYPE)* p_stk);

//------------------------------------------------------------------------------
/*! @brief   Calculates the size of the structure stack without hash and second canary
 *
 *  @param   p_stk       Pointer to stack
 *
 *  @return  stack size for hash
 */

static size_t TEMPLATE(StackSizeForHash, TYPE) (TEMPLATE(stack, TYPE)* p_stk);

//------------------------------------------------------------------------------
/*! @brief   (THE BEST FUNCTION) Print the contents of the stack and its data to the logfile
 *
 *  @param   p_stk       Pointer to stack
 *  @param   funcname    Name of the function from which the StackDump was called
 *  @param   logname     Name of logfile
 *
 *  @return  error code
 */

static error_t TEMPLATE(StackDump, TYPE) (TEMPLATE(stack, TYPE)* p_stk, const char* funcname = "@some function@", const char* logfile = logname);

//------------------------------------------------------------------------------
/*! @brief   Check stack for problems, canaries, hash (if enabled)
 *
 *  @param   p_stk       Pointer to stack
 *  @param   funcname    Name of the function from which the StackCheck was called
 *
 *  @return  error code
 */

static error_t TEMPLATE(StackCheck, TYPE) (TEMPLATE(stack, TYPE)* p_stk, const char* funcname);

//------------------------------------------------------------------------------
/*! @brief   Print information, error summary
 *
 *  @param   p_stk       Pointer to stack
 *  @param   fp          Pointer to logfile
 *
 *  @return  error code
 */

static void TEMPLATE(printError, TYPE) (TEMPLATE(stack, TYPE)* p_stk, FILE* fp);

//------------------------------------------------------------------------------
/*! @brief   Change canary (if enabled)
 *
 *  @param   canary      �anary input
 *
 *  @return  changed canary
 */

#ifdef CANARY_PROTECT

static can_t TEMPLATE(CanaryChange, TYPE) (can_t canary);

#endif // CANARY_PROTECT

//------------------------------------------------------------------------------
/*! @brief   Placing canaries in stack (if enabled)
 *
 *  @param   p_stk       Pointer to stack
 */

#ifdef CANARY_PROTECT

static void TEMPLATE(CanaryPlacing, TYPE) (TEMPLATE(stack, TYPE)* p_stk);

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

static int TEMPLATE(CanaryCheck, TYPE) (TEMPLATE(stack, TYPE)* p_stk);

#endif // CANARY_PROTECT


//==============================================================================
/*------------------------------------------------------------------------------
                   StackLib implementations                                    *
*///----------------------------------------------------------------------------
//==============================================================================


static error_t TEMPLATE(_StackConstruct, TYPE) (TEMPLATE(stack, TYPE)* p_stk, size_t capacity, char* stack_name)
{
    if ((p_stk->errCode != NOT_CONSTRUCTED) && (p_stk->errCode != STACK_DESTRUCTED))
    {
        printf(errstr[STACK_CONSTRUCTED + 1]);

        return STACK_CONSTRUCTED;
    }
    else if (capacity > MAX_CAPACITY)
    {
        printf("Wrong capacity value: %d - is too big\n", capacity);
        printf("impossible to create a stack\n");

        return CAPACITY_WRONG_VALUE;
    }
    else if (capacity == 0)
    {
        printf("Cannot create stack with capacity 0\n", capacity);
        printf("minimal capacity of stack is 1\n");

        return CAPACITY_WRONG_VALUE;
    }
    else if (stack_id == MAX_STACK_NUM)
    {
        printf("Cannot create new stack \n", capacity);
        printf("max number of stacks is %u\n", MAX_STACK_NUM);

        return NO_MEMORY;
    }

    void* temp = calloc(capacity * sizeof(TYPE) + 2 * sizeof(can_t), sizeof(char));

    if (temp == nullptr)
    {
        printf("\n%s", errstr[NO_MEMORY + 1]);
        printf("impossible to create a stack\n");

        return NO_MEMORY;
    }

    p_stk->data = (TYPE*)((char*)temp + sizeof(can_t));

    p_stk->size_cur = 0;
    p_stk->capacity = capacity;
    p_stk->name = (const char*)stack_name;

    TEMPLATE(StackPoison, TYPE) (p_stk);

    p_stk->errCode = OK;

    p_stk->id = stack_id;
    stack_id++;


#ifdef CANARY_PROTECT
    perfect_canary = TEMPLATE(CanaryChange, TYPE) (perfect_canary);
    canaries[p_stk->id] = perfect_canary;
    TEMPLATE(CanaryPlacing, TYPE) (p_stk);
#endif // CANARY_PROTECT

#ifdef HASH_PROTECT
    p_stk->datahash = hash(p_stk->data, p_stk->capacity * sizeof(TYPE));
    p_stk->stackhash = hash(p_stk, TEMPLATE(StackSizeForHash, TYPE) (p_stk));
#endif // HASH_PROTECT

    ASSERTOK(p_stk);

    DUMP_PRINT{ TEMPLATE(StackDump, TYPE) (p_stk, __FUNCTION__); }

    return OK;
}

//------------------------------------------------------------------------------

static error_t TEMPLATE(StackDestruct, TYPE) (TEMPLATE(stack, TYPE)* p_stk)
{
    ASSERTOK(p_stk);

    p_stk->size_cur = 0;

    TEMPLATE(StackPoison, TYPE) (p_stk);

    free((char*)p_stk->data - sizeof(can_t));

    p_stk->capacity = 0;

#ifdef CANARY_PROTECT
    p_stk->canary1 = 0;
    p_stk->canary2 = 0;
#endif // CANARY_PROTECT

#ifdef HASH_PROTECT
    p_stk->datahash = 0;
    p_stk->stackhash = 0;
#endif // HASH_PROTECT

    p_stk->data = nullptr;
    p_stk->errCode = STACK_DESTRUCTED;

    return OK;
}

//------------------------------------------------------------------------------

static error_t TEMPLATE(StackPush, TYPE) (TEMPLATE(stack, TYPE)* p_stk, TYPE value)
{
    ASSERTOK(p_stk);

    if (p_stk->size_cur == p_stk->capacity - 1)
    {
        if (TEMPLATE(StackExpand, TYPE) (p_stk) == NO_MEMORY)
        {
            printf(errstr[NO_MEMORY + 1]);

            p_stk->errCode = OK;
            return NO_MEMORY;
        }
    }

    p_stk->data[p_stk->size_cur++] = value;


#ifdef CANARY_PROTECT
    canaries[p_stk->id] = TEMPLATE(CanaryChange, TYPE) (canaries[p_stk->id]);
    TEMPLATE(CanaryPlacing, TYPE) (p_stk);
#endif // CANARY_PROTECT

#ifdef HASH_PROTECT
    p_stk->datahash = hash(p_stk->data, p_stk->capacity * sizeof(TYPE));
    p_stk->stackhash = hash(p_stk, TEMPLATE(StackSizeForHash, TYPE) (p_stk));
#endif // HASH_PROTECT

    ASSERTOK(p_stk);

    DUMP_PRINT{ TEMPLATE(StackDump, TYPE) (p_stk, __FUNCTION__); }

    return OK;
}

//------------------------------------------------------------------------------

static TYPE TEMPLATE(StackPop, TYPE) (TEMPLATE(stack, TYPE)* p_stk)
{
    ASSERTOK(p_stk);

    if (p_stk->errCode == EMPTY_STACK)
    {
        printf(errstr[EMPTY_STACK + 1]);

        p_stk->errCode = OK;
        return TEMPLATE(TYPE, POISON);
    }

    TYPE value = p_stk->data[--p_stk->size_cur];

    p_stk->data[p_stk->size_cur] = TEMPLATE(TYPE, POISON);


#ifdef CANARY_PROTECT
    canaries[p_stk->id] = TEMPLATE(CanaryChange, TYPE) (canaries[p_stk->id]);
    TEMPLATE(CanaryPlacing, TYPE) (p_stk);
#endif // CANARY_PROTECT

#ifdef HASH_PROTECT
    p_stk->datahash = hash(p_stk->data, p_stk->capacity * sizeof(TYPE));
    p_stk->stackhash = hash(p_stk, TEMPLATE(StackSizeForHash, TYPE) (p_stk));
#endif // HASH_PROTECT

    ASSERTOK(p_stk);

    DUMP_PRINT{ TEMPLATE(StackDump, TYPE) (p_stk, __FUNCTION__); }

    return value;
}

//------------------------------------------------------------------------------

static void TEMPLATE(StackPoison, TYPE) (TEMPLATE(stack, TYPE)* p_stk)
{
    assert(p_stk);
    assert(p_stk->data);
    assert(p_stk->size_cur < p_stk->capacity);

    for (int i = p_stk->size_cur; i < p_stk->capacity; ++i)
    {
        p_stk->data[i] = TEMPLATE(TYPE, POISON);
    }
}

//------------------------------------------------------------------------------

static int TEMPLATE(isPOISON, TYPE) (TYPE value)
{
    if (isnan((double)TEMPLATE(TYPE, POISON)))
        if (isnan((double)value))
            return 1;
        else
            return 0;
    return (value == TEMPLATE(TYPE, POISON));
}

//------------------------------------------------------------------------------

static error_t TEMPLATE(StackExpand, TYPE) (TEMPLATE(stack, TYPE)* p_stk)
{
    p_stk->capacity *= 2;

    void* temp = calloc(p_stk->capacity * sizeof(TYPE) + 2 * sizeof(can_t), sizeof(char));
    if (temp == nullptr)
        return NO_MEMORY;

    void* oldtemp = (char*)p_stk->data - sizeof(can_t);
    memcpy(temp, (char*)p_stk->data - sizeof(can_t), p_stk->capacity * sizeof(TYPE) + 2 * sizeof(can_t));
    free(oldtemp);

    p_stk->data = (TYPE*)((char*)temp + sizeof(can_t));

    TEMPLATE(StackPoison, TYPE) (p_stk);

    return OK;
}

//------------------------------------------------------------------------------

static size_t TEMPLATE(StackSizeForHash, TYPE) (TEMPLATE(stack, TYPE)* p_stk)
{
    assert(p_stk != nullptr);

    size_t size = 0;

    size += sizeof(p_stk->canary1);
    size += sizeof(p_stk->capacity);
    size += sizeof(p_stk->size_cur);
    size += sizeof(p_stk->name);
    size += sizeof(p_stk->data);
    size += sizeof(p_stk->errCode);
    size += sizeof(p_stk->id);

    return size;
}

//------------------------------------------------------------------------------

static error_t TEMPLATE(StackDump, TYPE) (TEMPLATE(stack, TYPE)* p_stk, const char* funcname, const char* logfile)
{
    const size_t linelen = 80;
    char divline[linelen + 1] = "********************************************************************************";

    FILE* fp = fopen(logfile, "a");
    if (fp == nullptr)
        return NOT_OK;

    if (funcname != nullptr)
        fprintf(fp, "This dump was called from a function %s\n", funcname);

    SYSTEMTIME localtime = {};
    GetLocalTime(&localtime);

    fprintf(fp, "TIME: %d-%02d-%02d %02d:%02d:%02d.%03d\n\n",
        localtime.wYear,
        localtime.wMonth,
        localtime.wDay,
        localtime.wHour,
        localtime.wMinute,
        localtime.wSecond,
        localtime.wMilliseconds);

    if (p_stk == nullptr)
    {
        fprintf(fp, "Stack (ERROR) [0x%p] \"unidentified stack\"\n", p_stk);
        fprintf(fp, errstr[NULL_STACK_PTR + 1]);

        fprintf(fp, "%s\n", divline);
        fclose(fp);

        return OK;
    }

    if ((p_stk->errCode == NOT_CONSTRUCTED) ||
        (p_stk->errCode == STACK_DESTRUCTED) ||
        (p_stk->errCode == NULL_DATA_PTR) ||
        (p_stk->errCode == SIZE_BIGGER_CAPACITY) ||
        (p_stk->errCode == CAPACITY_WRONG_VALUE))
    {
        fprintf(fp, "Stack (ERROR) [0x%p] \"%s\" id (%d)\n", p_stk, p_stk->name, p_stk->id);
        TEMPLATE(printError, TYPE) (p_stk, fp);

        fprintf(fp, "%s\n", divline);
        fclose(fp);

        return OK;
    }

    char* StkState = (char*)errstr[OK + 1];

    if (p_stk->errCode)
    {
        if ((p_stk->errCode == CANARY_DIED) || (p_stk->errCode == INCORRECT_HASH))
            StkState = (char*)errstr[NOT_OK + 1];

        TEMPLATE(printError, TYPE) (p_stk, fp);
    }

    fprintf(fp, "Stack (%s) [0x%p] \"%s\", id (%d)\n", StkState, p_stk, p_stk->name, p_stk->id);
    fprintf(fp, "\t{\n");

    fprintf(fp, "\tType of data is %s\n\n", TEMPLATE(TYPE, PRINT_TYPE));

    fprintf(fp, "\tCapacity           = %u\n", p_stk->capacity);
    fprintf(fp, "\tCurrent size       = %u\n\n", p_stk->size_cur);

#ifdef CANARY_PROTECT
    fprintf(fp, "\tCanary stack 1     = " CAN_PRINT_FORMAT "\n", p_stk->canary1);
    fprintf(fp, "\tCanary stack 2     = " CAN_PRINT_FORMAT "\n\n", p_stk->canary2);
#endif // CANARY_PROTECT

#ifdef HASH_PROTECT
    fprintf(fp, "\tStack hash         = 0x" HASH_PRINT_FORMAT "\n", p_stk->stackhash);
    fprintf(fp, "\tData hash          = 0x" HASH_PRINT_FORMAT "\n\n", p_stk->datahash);

    if (p_stk->errCode)
    {
        fprintf(fp, "\tTrue stack hash    = 0x" HASH_PRINT_FORMAT "\n", hash(p_stk, TEMPLATE(StackSizeForHash, TYPE) (p_stk)));
        fprintf(fp, "\tTrue data hash     = 0x" HASH_PRINT_FORMAT "\n\n", hash(p_stk->data, p_stk->capacity * sizeof(TYPE)));
    }
#endif // HASH_PROTECT

    fprintf(fp, "\tData [0x%p]\n", p_stk->data);
    fprintf(fp, "\t\t{\n");

#ifdef CANARY_PROTECT
    fprintf(fp, "\t\tCanary data 1: " CAN_PRINT_FORMAT "\n", ((can_t*)p_stk->data)[-1]);
#endif // CANARY_PROTECT

    for (int i = 0; i < p_stk->capacity; i++)
    {
        if (!(TEMPLATE(isPOISON, TYPE) (p_stk->data[i])))
            fprintf(fp, "\t\t*[%d]: " TEMPLATE(TYPE, PRINT_FORMAT) "\n", i, p_stk->data[i]);
        else
            fprintf(fp, "\t\t [%d]: " TEMPLATE(TYPE, PRINT_FORMAT) " (POISON)\n", i, p_stk->data[i]);
    }

#ifdef CANARY_PROTECT
    fprintf(fp, "\t\tCanary data 2: " CAN_PRINT_FORMAT "\n", ((can_t*)p_stk->data)[p_stk->capacity * sizeof(TYPE) / sizeof(can_t)]);
#endif // CANARY_PROTECT

    fprintf(fp, "\t\t}\n");

    fprintf(fp, "\t}\n");

    fprintf(fp, "%s\n", divline);
    fclose(fp);

    return OK;
}

//------------------------------------------------------------------------------

static error_t TEMPLATE(StackCheck, TYPE) (TEMPLATE(stack, TYPE)* p_stk, const char* funcname)
{
    if (p_stk == nullptr)
    {
        return NULL_STACK_PTR;
    }

    if (p_stk->errCode == NOT_CONSTRUCTED)
    {
        p_stk->name = "unidentified stack";
        p_stk->id = -666;
        return NOT_CONSTRUCTED;
    }

    if (p_stk->errCode == STACK_DESTRUCTED)
    {
        return STACK_DESTRUCTED;
    }

#ifdef HASH_PROTECT
    if (p_stk->stackhash != hash(p_stk, TEMPLATE(StackSizeForHash, TYPE) (p_stk)))
    {
        p_stk->errCode = INCORRECT_HASH;
        return INCORRECT_HASH;
    }
#endif // HASH_PROTECT

    if (p_stk->data == nullptr)
    {
        p_stk->errCode = NULL_DATA_PTR;
        return NULL_DATA_PTR;
    }

    if (p_stk->size_cur > p_stk->capacity)
    {
        p_stk->errCode = SIZE_BIGGER_CAPACITY;
        return SIZE_BIGGER_CAPACITY;
    }

    if ((p_stk->size_cur == 0) && (funcname == "StackPop_" TEMPLATE(TYPE, PRINT_TYPE)))
    {
        p_stk->errCode = EMPTY_STACK;
        return OK;
    }

    if ((p_stk->capacity == 0) || (p_stk->capacity > MAX_CAPACITY))
    {
        p_stk->errCode = CAPACITY_WRONG_VALUE;
        return CAPACITY_WRONG_VALUE;
    }

    if (!(TEMPLATE(isPOISON, TYPE) (p_stk->data[p_stk->size_cur])))
    {
        p_stk->errCode = WRONG_CUR_SIZE;
        return WRONG_CUR_SIZE;
    }

#ifdef CANARY_PROTECT
    if (TEMPLATE(CanaryCheck, TYPE) (p_stk))
    {
        p_stk->errCode = CANARY_DIED;
        return CANARY_DIED;
    }
#endif // CANARY_PROTECT

#ifdef HASH_PROTECT
    if (p_stk->datahash != hash(p_stk->data, p_stk->capacity * sizeof(TYPE)))
    {
        p_stk->errCode = INCORRECT_HASH;
        return INCORRECT_HASH;
    }
#endif // HASH_PROTECT

    else
    {
        p_stk->errCode = OK;
        return OK;
    }
}

//------------------------------------------------------------------------------

static void TEMPLATE(printError, TYPE) (TEMPLATE(stack, TYPE)* p_stk, FILE* fp)
{
    assert(fp != nullptr);

    if (p_stk == nullptr)
    {
        CONSOLE_PRINT{ printf(errstr[NULL_STACK_PTR + 1]); }
    }

    else if (p_stk->errCode != OK)
    {
        CONSOLE_PRINT{ printf(errstr[p_stk->errCode + 1]); }

        fprintf(fp, errstr[p_stk->errCode + 1]);
    }

    else if (p_stk->errCode == OK)
    {
        /* WOW! Program works correctly! eeeeeeeeeeeeeeeeee */
    }
    else { assert(0); }
}

//------------------------------------------------------------------------------

static can_t TEMPLATE(CanaryChange, TYPE) (can_t canary)
{
    return (canary * 2 + rand() * (rand() % 3 - 1)) % ULLONG_MAX;
}

//------------------------------------------------------------------------------

static void TEMPLATE(CanaryPlacing, TYPE) (TEMPLATE(stack, TYPE)* p_stk)
{
    assert(p_stk != nullptr);

    p_stk->canary1 = canaries[p_stk->id];
    p_stk->canary2 = canaries[p_stk->id];

    ((can_t*)p_stk->data)[-1] = canaries[p_stk->id];
    ((can_t*)p_stk->data)[p_stk->capacity * sizeof(TYPE) / sizeof(can_t)] = canaries[p_stk->id];
}

//------------------------------------------------------------------------------

static int TEMPLATE(CanaryCheck, TYPE) (TEMPLATE(stack, TYPE)* p_stk)
{
    if (p_stk->canary1 != canaries[p_stk->id])
        return NOT_OK;

    if (p_stk->canary2 != canaries[p_stk->id])
        return NOT_OK;

    if (((can_t*)p_stk->data)[-1] != canaries[p_stk->id])
        return NOT_OK;

    if (((can_t*)p_stk->data)[p_stk->capacity * sizeof(TYPE) / sizeof(can_t)] != canaries[p_stk->id])
        return NOT_OK;

    return OK;
}

//------------------------------------------------------------------------------
