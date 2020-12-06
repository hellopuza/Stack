/*------------------------------------------------------------------------------
    * File:        Stack.cpp                                                   *
    * Description: Implementations of stack functions                          *
    * Created:     1 dec 2020                                                  *
    * Copyright:   (C) 2020 MIPT                                               *
    * Author:      Artem Puzankov                                              *
    * Email:       puzankov.ao@phystech.edu                                    *
*///----------------------------------------------------------------------------

#include "Stack.h"

//------------------------------------------------------------------------------

error_t TEMPLATE(_StackConstruct, TYPE) (TEMPLATE(stack, TYPE)* p_stk, size_t capacity, char* stack_name)
{
    if (capacity > MAX_CAPACITY)
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

    void* temp = calloc(capacity * sizeof(TYPE) + 2 * sizeof(can_t), sizeof(char));
    
    if (temp == nullptr)
    {
        printf("\n%s", errstr[NO_MEMORY]);
        printf("impossible to create a stack\n");

        return NO_MEMORY;
    }

    p_stk->data = (TYPE*)((char*)temp + sizeof(can_t));

    p_stk->size_cur = 0;
    p_stk->capacity = capacity;
    p_stk->name     = (const char*)stack_name;

    TEMPLATE(StackPoison, TYPE) (p_stk);

    p_stk->errCode = OK;


#ifdef CANARY_PROTECT
    perfect_canary = CanaryChange(perfect_canary);
    TEMPLATE(CanaryPlacing, TYPE) (p_stk);
#endif // CANARY_PROTECT

#ifdef HASH_PROTECT
    p_stk->datahash  = (hash_t*)calloc(HASH_SIZE, 1);
    p_stk->stackhash = (hash_t*)calloc(HASH_SIZE, 1);
    p_stk->datahash[0]  = hash(p_stk->data, p_stk->capacity * sizeof(TYPE));
    p_stk->stackhash[0] = hash(p_stk, sizeof(struct Stack));
#endif // HASH_PROTECT

    ASSERTOK

    DUMP_PRINT{ TEMPLATE(StackDump, TYPE) (p_stk, __FUNCTION__); }

    return OK;
}

//------------------------------------------------------------------------------

error_t TEMPLATE(StackDestruct, TYPE) (TEMPLATE(stack, TYPE)* p_stk)
{
    ASSERTOK

    if (p_stk->errCode == STACK_DESTRUCTED)
    {
        printf(errstr[STACK_DESTRUCTED]);
        return STACK_DESTRUCTED;
    }
    p_stk->size_cur = 0;

    TEMPLATE(StackPoison, TYPE) (p_stk);

    free((char*)p_stk->data - sizeof(can_t));

    p_stk->capacity = 0;

#ifdef CANARY_PROTECT
    p_stk->canary1 = 0;
    p_stk->canary2 = 0;
#endif // CANARY_PROTECT

#ifdef HASH_PROTECT
    p_stk->datahash[0]  = 0;
    p_stk->stackhash[0] = 0;
    free(p_stk->datahash);
    free(p_stk->stackhash);
#endif // HASH_PROTECT

    p_stk->data = nullptr;

    return OK;
}

//------------------------------------------------------------------------------

error_t TEMPLATE(StackPush, TYPE) (TEMPLATE(stack, TYPE)* p_stk, TYPE value)
{
    ASSERTOK

    if (p_stk->size_cur == p_stk->capacity - 1)
    {
        if (TEMPLATE(StackExpand, TYPE) (p_stk) == NO_MEMORY)
        {
            printf(errstr[NO_MEMORY]);

            p_stk->errCode = OK;
            return NO_MEMORY;
        }
    }

    p_stk->data[p_stk->size_cur++] = value;


#ifdef CANARY_PROTECT
    perfect_canary = CanaryChange(perfect_canary);
    TEMPLATE(CanaryPlacing, TYPE) (p_stk);
#endif // CANARY_PROTECT

#ifdef HASH_PROTECT
    p_stk->datahash[0]  = hash(p_stk->data, p_stk->capacity * sizeof(TYPE));
    p_stk->stackhash[0] = hash(p_stk, sizeof(struct Stack));
#endif // HASH_PROTECT

    ASSERTOK

    DUMP_PRINT{ TEMPLATE(StackDump, TYPE) (p_stk, __FUNCTION__); }
    
    return OK;
}

//------------------------------------------------------------------------------

TYPE TEMPLATE(StackPop, TYPE) (TEMPLATE(stack, TYPE)* p_stk)
{
    ASSERTOK

    if (p_stk->errCode == EMPTY_STACK)
    {
        printf(errstr[EMPTY_STACK]);

        p_stk->errCode = OK;
        return TEMPLATE(TYPE, POISON);
    }

    TYPE value = p_stk->data[--p_stk->size_cur];

    p_stk->data[p_stk->size_cur] = TEMPLATE(TYPE, POISON);


#ifdef CANARY_PROTECT
    perfect_canary = CanaryChange(perfect_canary);
    TEMPLATE(CanaryPlacing, TYPE) (p_stk);
#endif // CANARY_PROTECT

#ifdef HASH_PROTECT
    p_stk->datahash[0]  = hash(p_stk->data, p_stk->capacity * sizeof(TYPE));
    p_stk->stackhash[0] = hash(p_stk, sizeof(struct Stack));
#endif // HASH_PROTECT

    ASSERTOK

    DUMP_PRINT{ TEMPLATE(StackDump, TYPE) (p_stk, __FUNCTION__); }

    return value;
}

//------------------------------------------------------------------------------

void TEMPLATE(StackPoison, TYPE) (TEMPLATE(stack, TYPE)* p_stk)
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

error_t TEMPLATE(StackExpand, TYPE) (TEMPLATE(stack, TYPE)* p_stk)
{
    ASSERTOK

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

error_t TEMPLATE(StackDump, TYPE) (TEMPLATE(stack, TYPE)* p_stk, const char* funcname, const char* logfile)
{
    const size_t linelen = 80;
    char divline[linelen + 1] = "********************************************************************************";

    FILE* fp = fopen(logfile, "a");
    if (fp == nullptr)
        return NOT_OK;

    if (funcname != nullptr)
        fprintf(fp, "this dump was called from a function %s\n", funcname);

    SYSTEMTIME localtime;
    GetLocalTime(&localtime);

    fprintf(fp, "TIME: %d-%02d-%02d %02d:%02d:%02d.%03d\n",
        localtime.wYear,
        localtime.wMonth,
        localtime.wDay,
        localtime.wHour,
        localtime.wMinute,
        localtime.wSecond,
        localtime.wMilliseconds);


    if ( (p_stk->errCode == NULL_STACK_PTR)       ||
         (p_stk->errCode == NULL_DATA_PTR)        ||
         (p_stk->errCode == SIZE_BIGGER_CAPACITY) ||
         (p_stk->errCode == CAPACITY_WRONG_VALUE)    )
    {
        fprintf(fp, "Stack (ERROR) [0x%p] \"%s\"\n", p_stk, p_stk->name);
        TEMPLATE(printError, TYPE) (p_stk, fp);

        fprintf(fp, "%s\n", divline);
        fclose(fp);

        return OK;
    }
    
    char* StkState = (char*)errstr[OK];

    if (p_stk->errCode)
    {
        if ((p_stk->errCode == CANARY_DIED) || (p_stk->errCode == INCORRECT_HASH))
            StkState = (char*)errstr[NOT_OK];
        
        TEMPLATE(printError, TYPE) (p_stk, fp);
    }

    fprintf(fp, "Stack (%s) [0x%p] \"%s\"\n", StkState, p_stk, p_stk->name);
    fprintf(fp, "\t{\n");

        fprintf(fp, "\tType of data is %s\n", TEMPLATE(TYPE, PRINT_TYPE));
        fprintf(fp, "\tCapacity           = %d\n",   p_stk->capacity);
        fprintf(fp, "\tCurrent size       = %d\n\n", p_stk->size_cur);

#ifdef CANARY_PROTECT
        fprintf(fp, "\tCanary stack 1     = " CAN_PRINT_FORMAT "\n",   p_stk->canary1);
        fprintf(fp, "\tCanary stack 2     = " CAN_PRINT_FORMAT "\n\n", p_stk->canary2);
#endif // CANARY_PROTECT

#ifdef HASH_PROTECT
        fprintf(fp, "\tStack hash         = 0x" HASH_PRINT_FORMAT "\n",   *(p_stk->stackhash));
        fprintf(fp, "\tData hash          = 0x" HASH_PRINT_FORMAT "\n\n", *(p_stk->datahash));
        
        if (p_stk->errCode)
        {
        fprintf(fp, "\tTrue stack hash    = 0x" HASH_PRINT_FORMAT "\n",   hash(p_stk, sizeof(struct Stack)));
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
            if (i < p_stk->size_cur)
                fprintf(fp, "\t\t*[%d]: " TEMPLATE(TYPE, PRINT_FORMAT) "\n",          i, p_stk->data[i]);
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

error_t TEMPLATE(StackCheck, TYPE) (TEMPLATE(stack, TYPE)* p_stk, const char* funcname)
{
    if (p_stk == nullptr)
    {
        p_stk = (TEMPLATE(stack, TYPE)*) calloc(sizeof(struct Stack), 1);
        p_stk->errCode = NULL_STACK_PTR;
        return NULL_STACK_PTR;
    }

    if ((p_stk->data == nullptr) && (funcname == "StackDestruct_" TEMPLATE(TYPE, PRINT_TYPE)))
    {
        p_stk->errCode = STACK_DESTRUCTED;
        return OK;
    }

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

#ifdef HASH_PROTECT
    if (   (p_stk->stackhash[0] != hash(p_stk, sizeof(struct Stack)))
        || (p_stk->datahash[0]  != hash(p_stk->data, p_stk->capacity * sizeof(TYPE))) )
    {
        p_stk->errCode = INCORRECT_HASH;
        return INCORRECT_HASH;
    }
#endif // HASH_PROTECT

#ifdef CANARY_PROTECT
    if (TEMPLATE(CanaryCheck, TYPE) (p_stk))
    {
        p_stk->errCode = CANARY_DIED;
        return CANARY_DIED;
    }
#endif // CANARY_PROTECT

    else
    {
        p_stk->errCode = OK;
        return OK;
    }
}

//------------------------------------------------------------------------------

void TEMPLATE(printError, TYPE) (TEMPLATE(stack, TYPE)* p_stk, FILE* fp)
{
    assert(fp != nullptr);

    if (p_stk == nullptr)
    {
        CONSOLE_PRINT{ printf(errstr[NULL_STACK_PTR]); }
    }

    else if (p_stk->errCode != OK)
    {
        CONSOLE_PRINT { printf(errstr[p_stk->errCode]); }

        fprintf(fp, errstr[p_stk->errCode]);
    }

    else if (p_stk->errCode == OK)
    {
        /* WOW! Program works correctly! eeeeeeeeeeeeeeeeee */
    }
    else { assert(0); }
}

//------------------------------------------------------------------------------

can_t CanaryChange(can_t canary)
{
    return (perfect_canary * 2 + rand() * (rand() % 3 - 1)) % ULLONG_MAX;
}

//------------------------------------------------------------------------------

void TEMPLATE(CanaryPlacing, TYPE) (TEMPLATE(stack, TYPE)* p_stk)
{
    assert(p_stk != nullptr);

    p_stk->canary1 = perfect_canary;
    p_stk->canary2 = perfect_canary;
    
    ((can_t*)p_stk->data)[-1] = perfect_canary;
    ((can_t*)p_stk->data)[p_stk->capacity * sizeof(TYPE) / sizeof(can_t)] = perfect_canary;
}

//------------------------------------------------------------------------------

int TEMPLATE(CanaryCheck, TYPE) (TEMPLATE(stack, TYPE)* p_stk)
{
    if (p_stk->canary1 != perfect_canary)
        return NOT_OK;

    if (p_stk->canary2 != perfect_canary)
        return NOT_OK;

    if (((can_t*)p_stk->data)[-1] != perfect_canary)
        return NOT_OK;

    if (((can_t*)p_stk->data)[p_stk->capacity * sizeof(TYPE) / sizeof(can_t)] != perfect_canary)
        return NOT_OK;

    return OK;
}

//------------------------------------------------------------------------------
